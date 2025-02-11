#include "../Includes/Bot.hpp"

int g_sigint = 0;

int atoi(char *str)
{
    std::istringstream iss(str);
    int result;

    iss >> result;
    return (result);
}

void sigHandler(int signal)
{
    (void)signal;
    g_sigint = 1;
    exit(0);
}

int Bot::signals()
{
    signal(SIGINT, sigHandler);
    return(0);
}

int main(int ac, char **av)
{
    signal(SIGINT, sigHandler);
    try
    {
        if (ac != 4)
            throw std::runtime_error("Please enter ./bot <port> <channel_name> <psswd>");
        Bot bot(atoi(av[1]), av[2], av[3]);
        bot.setBot();
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }
    return (0);
}