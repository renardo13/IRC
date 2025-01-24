#pragma once
//Gerenal error

#define ERR_INVALID_ARGUMENTS "Error: Invalid arguments provided"
#define ERR_MEMORY_ALLOCATION "Error: Failed to allocate memory"
#define ERR_UNKNOWN_ERROR "Error: An unknown error occurred"
#define ERR_PERMISSION_DENIED "Error: Permission denied"


// Network error

#define ERR_SOCKET_CREATION "Error: Failed to create socket"
#define ERR_SOCKET_BINDING "Error: Failed to bind socket"
#define ERR_SOCKET_LISTENING "Error: Failed to set socket to listening mode"
#define ERR_SOCKET_ACCEPT "Error: Failed to accept new connection"
#define ERR_SOCKET_SEND "Error: Failed to send data"
#define ERR_SOCKET_RECEIVE "Error: Failed to receive data"
#define ERR_SOCKET_CLOSE "Error: Failed to close socket"
#define ERR_POLL_FAILURE "Error: poll() failed"
#define ERR_CONNECTION_LIMIT "Error: Connection limit reached"

// Error of the irc protocol

#define ERR_INVALID_COMMAND "Error: Invalid command received"
#define ERR_UNKNOWN_COMMAND "Error: Unknown command received"
#define ERR_NOT_REGISTERED "Error: Client not registered"
#define ERR_ALREADY_REGISTERED "Error: Client already registered"
#define ERR_CHANNEL_NOT_FOUND "Error: Channel does not exist"
#define ERR_CHANNEL_FULL "Error: Channel is full"
#define ERR_NO_CHANNEL_JOINED "Error: No channel joined"
#define ERR_USER_NOT_FOUND "Error: User not found"
#define ERR_USER_BANNED "Error: User is banned from this channel"
#define ERR_INVALID_NICKNAME "Error: Invalid nickname format"
#define ERR_NICKNAME_IN_USE "Error: Nickname is already in use"

// File error

#define ERR_FILE_OPEN "Error: Failed to open file"
#define ERR_FILE_READ "Error: Failed to read from file"
#define ERR_FILE_WRITE "Error: Failed to write to file"
#define ERR_FILE_CLOSE "Error: Failed to close file"

// Limits error

#define ERR_NOT_AN_OPERATOR "Error: You must be an operator to perform this action"
#define ERR_INVALID_PASSWORD "Error: Invalid password"
#define ERR_COMMAND_NOT_ALLOWED "Error: You are not allowed to execute this command"




