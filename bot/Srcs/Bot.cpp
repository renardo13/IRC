#include "../Includes/Bot.hpp"
#include "../../server/Includes/colors.hpp"

/* No need to user NONBLOCKANT flag because we use poll that allows us to know when datas are received */

Bot::Bot(int port, std::string channel, std::string psswd) : port(port), channel(channel), psswd(psswd), name("Julia"), admin(false), start(0)
{
    srand(time(0));
    memset(&server_addr, 0, sizeof(server_addr));
    adversary_point = 0;
    bot_point = 0;
}

Bot::~Bot() {}

void Bot::connectToServer()
{
    fds[0].fd = pfd;
    fds[0].events = POLLIN;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    if (connect(pfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
        throw std::runtime_error("Bot failed to connect to server");
    sendMsgToServer(RPL_JOIN);
}

std::string Bot::randomChoice()
{
    int i = rand() % 3;
    if (i == 0)
        return ("SCISSORS");
    else if (i == 1)
        return ("PAPER");
    return ("ROCK");
}

void Bot::dual(std::string adversary_rply)
{
    std::string bot_rply = randomChoice();
    sendMsgToServer(RPL_IN_CHAN(channel, bot_rply));
    if (bot_rply != adversary_rply)
    {
        if (bot_rply == "SCISSORS" && adversary_rply == "PAPER")
            bot_point++;
        else if (bot_rply == "ROCK" && adversary_rply == "SCISSORS")
            bot_point++;
        else if (bot_rply == "PAPER" && adversary_rply == "ROCK")
            bot_point++;
        else
            adversary_point++;
    }
}

void Bot::play(std::string user)
{
    Command cmd;
    char buffer[512];
    int nbytes = 0;
    int ret = 0;
    while (!g_sigint && adversary_point < 3 && bot_point < 3)
    {
        ret = poll(fds, 1, 100000);
        if (ret == 0)
            continue;
        if (ret == -1)
        {
            if (g_sigint)
                break;
            throw std::runtime_error("Error in poll()");
        }
        if (fds[0].revents & POLLIN)
        {
            nbytes = recv(pfd, buffer, 512, 0);
            if (nbytes == -1)
                throw std::runtime_error("Error while receiving data from server");
            buffer[nbytes] = '\0';
            std::cout << std::endl
                      << BOLD << RED << "[SERVER] => " << buffer << RESET;
            cmd.parseCmd(buffer);
            std::string command = cmd.getMsg().substr(0, cmd.getMsg().size() - 2);
            std::cout << "COMMAND = " << command << std::endl;
            if (command == "ROCK" || command == "PAPER" || command == "SCISSORS")
                dual(command);
        }
    }
    if (adversary_point == 3)
        sendMsgToServer(RPL_IN_CHAN(channel, "Good game, you earn my respect"));
    else
        sendMsgToServer(RPL_KICK(channel, user, "You are a looser"));
    adversary_point = 0;
    bot_point = 0;
}

bool Bot::amIAdmin(std::string buffer)
{
    int i = 0;
    while (buffer[i])
    {
        if (buffer[i] == '@' && buffer[i - 1] == ':')
        {
            std::cout << "HLOOOOO" << "{" << buffer[i + name.size()] << "}\n";
            std::string admin = buffer.substr(i + 1, buffer.find('\n', i + 1) - i);
            admin = admin.substr(0, admin.size() - 2);

            if (admin == name && buffer[i + name.size()] == '\n')
            {
                std::cout << "JE SUIS ADMIN\n";
                return (true);
            }
            break;
        }
        i++;
    }
    return (false);
}

void Bot::parseServMsg(std::string buffer)
{
    Command cmd;
    cmd.parseCmd(buffer);
    std::string command = cmd.getArg()[0];
    std::string user = "";
    if (command == "JOIN")
    {
        user = cmd.getCmd().substr(1, cmd.getCmd().find("!") - 1);
        if (user != name)
            sendMsgToServer(RPL_IN_CHAN(channel, "Welcome " + user + " please, play with me ! Type PLAY to play with me 🤗🤗"));
    }
    else if (command == "MODE")
    {
        user = cmd.getArg()[1].substr(0, cmd.getArg()[1].size() - 2);
        if (cmd.getMode()[0] == "+o" && user == name)
            admin = true;
        if (cmd.getMode()[0] == "-o" && user == name)
            admin = false;
    }
    else if (!cmd.getMsg().empty() && cmd.getMsg().substr(0, cmd.getMsg().size() - 2) == "PLAY")
    {
        sendMsgToServer(RPL_IN_CHAN(channel, "Ok let's play ROCK - PAPER - SCISSORS !"));
        sendMsgToServer(RPL_IN_CHAN(channel, "If you loose I kick you from the channel if I can!"));
        if (command == "PRIVMSG")
        {
            int start = cmd.getCmd().find(':') + 1;
            std::string user = cmd.getCmd().substr(start, cmd.getCmd().find('!', start) - 1);
            play(user);
        }
    }
}

void Bot::getMessages()
{
    char buffer[512];
    int nbytes = 0;
    int ret = 0;
    while (1)
    {
        ret = poll(fds, 1, 100000);
        if (g_sigint)
                break;
        if (ret == 0)
            continue;
        if (ret == -1)
            throw std::runtime_error("Error in poll()");
        if (fds[0].revents || POLLIN)
        {
            nbytes = recv(pfd, buffer, 512, 0);
            if (nbytes == -1)
                throw std::runtime_error("Error while receiving data from server");
            buffer[nbytes] = '\0';
            std::cout << std::endl
                      << BOLD << RED << "[SERVER] => " << buffer << RESET;
            parseServMsg(buffer);
        }
    } 
    sendMsgToServer("QUIT :leaving");
    close(pfd);
}

int Bot::setBot()
{
    pfd = socket(AF_INET, SOCK_STREAM, 0);
    connectToServer();
    getMessages();
    return (0);
}

void Bot::setPfd(int pfd)
{
    this->pfd = pfd;
}

int Bot::getPfd() const
{
    return (pfd);
}

int Bot::sendMsgToServer(std::string msg)
{
    std::cout << BOLD << YELLOW << "[CLIENT BOT] => " << msg << RESET << std::endl;
    std::string irc_msg = msg + "\r\n";
    return (send(pfd, irc_msg.c_str(), irc_msg.size(), 0));
}
