
#ifndef INCLUDE_RABBIT
#define INCLUDE_RABBIT

/***
 * @Author : Arijit Chattopadhyay
 ***/

#include <amqp.h>   
#include <amqp_framing.h>
#include <amqp_tcp_socket.h>
#include <iostream>
#include <string>
using namespace std;


class Rabbit {
	public:
		Rabbit(string host, string vhost, string username, string password, int port);
		~Rabbit();
		int initPublisher();
		int initConsumer();
		int publish(const string& msg);
		int consume();


	private:
		int connect();
		int declareExchange();
		int declareQueue();
		int bindQueue();

	private:
		amqp_socket_t *d_socket; 
		amqp_connection_state_t d_connection;
		string d_host;
		string d_vhost; 
		string d_username; 
		string d_password; 
		string d_exchangeName;
		string d_queueName; 
		string d_routingKey;
		int d_port;                
		int d_channel;
		bool d_isPublisher;

};


#endif
