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

Command Command::parseCmd(std::string msg)
{
    Command cmd;

    cmd.cmd = msg.substr(msg.find(' '));
    cmd.cmd = cmd.cmd.substr(cmd.cmd.find_first_not_of(' '));
    std::cout <<  "cmd.cmd " << cmd.cmd << std::endl;

    for(int i = 0; i < (int)msg.size(); i++)
    {
        if(msg[i] == '#')
        {
            size_t channel_end = msg.find(' ');
            std::string channel_name = msg.substr(i + 1, channel_end - 1);
            cmd.channel.push_back(channel_name);

        }
    }

	// std::string chan = mess.substr(mess.find_first_not_of(' '));

    std::vector<std::string>::iterator mode = cmd.mode.begin();
    std::cout << "Mode :" << std::endl;
    for(; mode != cmd.mode.end(); mode++ )
    {   
        std::cout << *mode << std::endl;
    }

    std::cout << "Channel :" << std::endl;
    std::vector<std::string>::iterator channel = cmd.channel.begin();
    for(; channel != cmd.channel.end(); channel++ )
    {   
        std::cout << *channel << std::endl;
    }

    std::cout << "User :" << std::endl;
    std::vector<std::string>::iterator user = cmd.user.begin();
    for(; user != cmd.user.end(); user++ )
    {   
        std::cout << *user << std::endl;
    }

    std::cout << "Message = " << cmd.msg;


    return(cmd);
}