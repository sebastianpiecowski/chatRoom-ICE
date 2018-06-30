#include <stdio.h>
#include <stdlib.h>
#include "Command.cpp"
using namespace std;
using namespace Command;
using namespace Chat;

int main(int argc, char* argv[]) {
    int status = 0;
    
    string username;
    
    cout << "Enter your username: " << endl;
    cin >> username;

    Client client(username);

    while (true) {
        cout << endl;
        cout << "---------MENU---------" << endl;
        cout << "1. Create room" << endl;
        cout << "2. Join to room" << endl;
        cout << "3. Leave room" << endl;
        cout << "4. Get list of users in room" << endl;
        cout << "5. Get room list" << endl;
        cout << "6. Send message to room" << endl;
        cout << "7. Send private message to other user" << endl;
        cout << "0. Exit" << endl;
        cout << endl;
	int choice;
        cin >> choice; 
        while (choice < 0 || choice > 7) {
            cout << "Unkown option" << endl;
            cin >> choice;
        }
        switch (choice) {
            case 1:
                client.createRoom();
                break;
            case 2:
		client.joinToRoom();              
                break;
            case 3:
                client.leaveRoom();
                break;
            case 4:     
		client.printUsersInRoom();		
                break;
            case 5:
		client.printListAllRooms();
		break;
	    case 6:
		client.sendMessageToRoom();
                break;
            case 7:
                client.sendPrivateMessageToUser();
                break;      
            case 0:
                return status;
        }
    }
    return status;
}
