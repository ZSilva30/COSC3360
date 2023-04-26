// Zitlali Silva 
// COSC 3360
// Client.cpp 
// Citations: https://www.geeksforgeeks.org/udp-server-client-implementation-c/
          //  https://www.geeksforgeeks.org/socket-programming-cc/
           // Paris Explanation for Assignment 2

// Libaries use for the server, not sure if i need to take some off but i have them just in case
#include <iostream>
#include <cstring>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
using namespace std;
// we will define a buffer size
#define BUFFER_SIZE 1024 

int main() {
    // get server host name from user
    cout << "Enter the server host name: ";
    string server_hostname;
    cin >> server_hostname;

  // get port number from user
    cout << "Enter the server port number: ";
    uint16_t server_portNUM;
    cin >> server_portNUM;

    // Create a datagram socket
    int sockfd;
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) { cerr << "Error: could not create socket"; return 1; }

    // host name to IP address
    struct hostent *server = gethostbyname(server_hostname.c_str());
    if (server == NULL) { cerr << "Error: could not resolve host name"; return 1; }

    // Here i am setting up the server address structure
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    memcpy(&server_addr.sin_addr.s_addr, server->h_addr_list[0], server->h_length);
    server_addr.sin_port = htons(server_portNUM);

    // Getting the license plate number from user
    string license_plate;
    cout << "Enter a license plate number: ";
    cin >> license_plate;

    // Send license plate to the server
    int plates_sent;
    plates_sent= sendto(sockfd, license_plate.c_str(), license_plate.length(), 0, (struct sockaddr*)&server_addr, sizeof(server_addr));
    if (plates_sent < 0) { cerr << "Error: could not send message to server"; return 1; }

// Receive a reply from the server
char buffer[BUFFER_SIZE];
memset(buffer, 0, BUFFER_SIZE);
socklen_t server_addr_len = sizeof(server_addr);
int plates_received;
plates_received= recvfrom(sockfd, buffer, BUFFER_SIZE, 0, (struct sockaddr*)&server_addr, &server_addr_len);
if (plates_received < 0) { cerr << "Error: could not receive message from server"; return 1; }

// Display outcome 
cout << buffer << endl;

// Close socket and exit
  close(sockfd);
  return 0;
}