#include <Ice/Ice.h>
#include "Chat.h"
#include <stdio.h>
#include <stdlib.h>

using namespace Chat;
using namespace std;

Ice::CommunicatorPtr iceCommunicator;
Ice::ObjectAdapterPtr adapter;
ServerPrx server;
UserPrx user;
class IUser : public User {
            public:
                IUser(const string& n) : name(n){};
                virtual string getName(const Ice::Current&) override;
                virtual void SendMessageToRoom(const string& room, const UserPrx& who,const string& message,const ::Ice::Current&) override;
                virtual void SendPriv(const UserPrx& who, 
                                                const string& message, 
                                                const ::Ice::Current&) override;
            private:
                string name;
};
    string IUser::getName(const Ice::Current&) { 
        return name;
    }

    void IUser::SendMessageToRoom(const string& room, const UserPrx& who, const string& message,const ::Ice::Current&) {
		cout<< "Room:"+ room+ "->" << who->getName()+ ":" << message << endl;
    }

    void IUser::SendPriv(const UserPrx& who, 
                                    const string& message, 
                                    const ::Ice::Current&) {
		cout<< "Priv from->" << who->getName()+ ":" << message << endl;
        
    }
