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

void Command::parseCmd(std::string msg)
{
    size_t end;

    end = msg.find_first_not_of(' ');
    size_t next_space = msg.find(' ', end);

    if (next_space == std::string::npos)
        cmd = msg.substr(end);
    else
        cmd = msg.substr(end, next_space - end);

    std::cout << "MESSAGE = " << msg << std::endl;

    for (int i = cmd.size(); i < (int)msg.size(); i++)
    {
        if (msg[i] == '#')
        {
            end = find_end(msg, i);
            channel.push_back(msg.substr(i + 1, end - i - 1));
            i = end;
        }
        else if (msg[i] == '+')
        {
            end = find_end(msg, i);
            mode.push_back(msg.substr(i, end - i));
            i = end - 1;
        }
        else if (msg[i] != ' ' && msg[i] != ':')
        {
            end = msg.find(' ', i);
            if (end == std::string::npos)
                end = msg.size();

            user.push_back(msg.substr(i, end - i));
            i = end - 1;
        }
        else if (msg[i] == ':')
        {
            msg = msg.substr(i + 1);
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
    // std::cout << std::endl;
     std::cout << "Message : " << msg << std::endl;
}

