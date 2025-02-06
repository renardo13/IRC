#include "../Includes/Command.hpp"

Command::Command(Command const &obj)
{
    if (this != &obj)
        *this = obj;
}

Command const &Command::operator=(Command const &obj)
{
    this->cmd = obj.cmd;
    this->channel = obj.channel;
    this->user = obj.user;
    this->msg = obj.msg;
    this->mode = obj.mode;
    return (*this);
}

size_t find_end(const std::string& msg, size_t start)
{
    size_t end1 = msg.find(' ', start);
    size_t end2 = msg.find(',', start);

    if (end1 == std::string::npos)
        end1 = msg.size();
    if (end2 == std::string::npos)
        end2 = msg.size();
    return std::min(end1, end2);
}

void Command::parseCmd(std::string message)
{
    size_t end;

    end = message.find_first_not_of(' ');
    size_t next_space = message.find(' ', end);
    memset(this, 0, sizeof(Command));
    if (next_space == std::string::npos)
        cmd = message.substr(end);
    else
        cmd = message.substr(end, next_space - end);

    std::cout << std::endl << BOLD << GREEN << "[IRSSI] => " << message << RESET << std::endl;
    for (int i = cmd.size(); i < (int)message.size(); i++)
    {
        if (message[i] == '#')
        {
            end = find_end(message, i);
            channel.push_back(message.substr(i + 1, end - i - 1));
            i = end;
        }
        else if (message[i] == '+' || message[i] == '-')
        {
            end = find_end(message, i);
            mode.push_back(message.substr(i, end - i));
            i = end - 1;
        }
        else if (message[i] != ' ' && message[i] != ':')
        {
            end = message.find(' ', i);
            if (end == std::string::npos)
                end = message.size();

            user.push_back(message.substr(i, end - i));
            i = end - 1;
        }
        else if (message[i] == ':')
        {
            msg = message.substr(i + 1);
            break;
        }
    }
    // std::cout << std::endl << "Mode : ";
    // for (std::vector<std::string>::iterator mode = cmd.mode.begin(); mode != cmd.mode.end(); mode++)
    // {
    //     std::cout << *mode << ", ";
    // }
    // std::cout << std::endl;
    // std::cout << "Channel : ";
    // for (std::vector<std::string>::iterator channel = this->channel.begin(); channel != this->channel.end(); channel++)
    // {
    //     std::cout << *channel << ", ";
    // }
    // std::cout << std::endl;
    // std::cout << "User : ";
    // for (std::vector<std::string>::iterator user = cmd.user.begin(); user != cmd.user.end(); user++)
    // {
    //     std::cout << *user  << ", ";
    // }
        std::cout << std::endl;
        std::cout << "Message in command : " << msg << std::endl;
}

