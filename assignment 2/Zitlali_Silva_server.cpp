// Zitlali Silva 
// COSC 3360
// Server.cpp 
// Citations: https://www.geeksforgeeks.org/udp-server-client-implementation-c/
          //  https://www.geeksforgeeks.org/socket-programming-cc/
           // Paris Explanation for Assignment 2

// Libaries use for the server, not sure if i need to take some off but i have them just in case
#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <set> 
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
using namespace std;

int main() {
  // First, Getting the file name from the user
    cout << "Enter today's stolen car DB name: ";
    string File_Name;
    getline(cin, File_Name);

    // Then, open the file and read the license plates into a "set"
    ifstream infile(File_Name.c_str());
    if (!infile.is_open()) {
        cerr << "Could not open file " << File_Name << endl;
        exit(1); }
  
    // Define the set here for stolen cars
    set<string> stolen_cars;
    string li_plate;
    // making a while loop
    while (infile >> li_plate) { stolen_cars.insert(li_plate); }
    infile.close(); // closing the file

    // Get the server port number from the user
    cout << "Enter server port number: ";
    int portNumber;
    cin >> portNumber;

    // Create a datagram socket
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("Error creating socket");
        exit(1); }

    // Bind the socket to the port number that user input
   //  In the <netinet/in.h> library 
    struct sockaddr_in Server_Addr, Client_Addr; 
    memset((char *)&Server_Addr, 0, sizeof(Server_Addr));
    Server_Addr.sin_family = AF_INET;  // host address
    Server_Addr.sin_addr.s_addr = INADDR_ANY;
    Server_Addr.sin_port = htons(portNumber); // set port number
  // making a if statement for the bind () 
    if (bind(sockfd, (struct sockaddr *)&Server_Addr, sizeof(Server_Addr)) < 0) {
        perror("Error binding socket");
        exit(1);
    }

    // we wait for the request from client side and to see if the plate is in the file/database
socklen_t Client_Length = sizeof(Client_Addr);
char buffer[256];
while (1) { int n = recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr *)&Client_Addr, &Client_Length);
    if (n < 0) {
        perror("Error receiving data");
        exit(1);
    }
// now we are going to report if the car has been stolen or not in the database
  buffer[n] = '\0';
    if (stolen_cars.count(buffer) > 0) {
      // if plate is found
    cout << buffer << ": Reported as stolen." << endl;
    const char* msg = "Plate reported as stolen";
    n = sendto(sockfd, msg, strlen(msg), 0, (struct sockaddr *)&Client_Addr, Client_Length);
      
} else if (strcmp(buffer, "killsvc") == 0) {
      // if user wants to terminate the code
    cout << "Received request to terminate service" << endl;
    cout << "Bye" << endl;
    const char* msg = "Client terminates. Bye!";
    n = sendto(sockfd, msg, strlen(msg), 0, (struct sockaddr *)&Client_Addr, Client_Length);
    exit(0);
} else {
      // plate is NOT FOUND
    cout << buffer << ": Not in the database." << endl;
    const char* msg = "Not in the database";
    n = sendto(sockfd, msg, strlen(msg), 0, (struct sockaddr *)&Client_Addr, Client_Length);
}

    if (n < 0) {
        perror("Error sending data");
        exit(1); }
}
// Close the socket
close(sockfd);
return 0; }