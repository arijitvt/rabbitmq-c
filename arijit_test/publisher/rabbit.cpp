#include <rabbit.h>


Rabbit::Rabbit(string host, string vhost,string username,string password, int port) 
:d_socket(NULL)
, d_host(host)
, d_vhost(vhost)
, d_username(username)
, d_password(password)
, d_port(port)
, d_channel(1)
, d_isPublisher(false) {
	d_exchangeName = "arijit";
	d_routingKey = d_exchangeName;


}  

Rabbit::~Rabbit() {
	amqp_channel_t channel(1);
	amqp_channel_close(d_connection,channel, AMQP_REPLY_SUCCESS);
	amqp_destroy_connection(d_connection);
}

int  Rabbit::initPublisher() 
{
	connect();
	declareExchange();
	d_isPublisher = true;
	return 0;
}

int Rabbit::initConsumer() 
{
	connect();
	declareExchange(); 
	declareQueue(); 
	bindQueue();
	d_isPublisher = false;
	return 0;
}

int Rabbit::connect() {
	cout << __func__ << endl;
	int rc = 0;
    d_connection = amqp_new_connection();
	d_socket = amqp_tcp_socket_new(d_connection);
	if(d_socket == NULL) {
		cerr << "Unable to create tcp socket" << endl;
		return -1;
	}

	rc = amqp_socket_open(d_socket, d_host.c_str(), d_port);
	if(rc) {
		cerr << " Unable to open tcp socket " << endl;
		return rc;
	}

	amqp_rpc_reply_t reply = amqp_login(d_connection,
			d_vhost.c_str(),
			0 ,
			131072, 
			0,
			AMQP_SASL_METHOD_PLAIN,
			d_username.c_str(),
			d_password.c_str());

	if(reply.reply_type != AMQP_RESPONSE_NORMAL) {
		cout << " Unable to do login with rc " << reply.reply_type << endl;
		return reply.reply_type;
	}

	amqp_channel_t channel (d_channel);
	amqp_channel_open(d_connection,channel);
	reply = amqp_get_rpc_reply(d_connection); 
	if(reply.reply_type != AMQP_RESPONSE_NORMAL) {
		cout << " Unable to open the channel " << reply.reply_type << endl;
		return reply.reply_type;
	}
	return rc; 
}

int Rabbit::declareExchange() {

	cout << __func__ << endl;

	amqp_channel_t channel(1);
	amqp_exchange_declare (d_connection,
			channel,
			amqp_cstring_bytes(d_exchangeName.c_str()),
			amqp_cstring_bytes("fanout"),
			amqp_boolean_t(0), //passive
			amqp_boolean_t(1), //durable
			amqp_boolean_t(0), //auto-delete
			amqp_boolean_t(0), //internal
			amqp_empty_table);

	amqp_rpc_reply_t reply = amqp_get_rpc_reply(d_connection); 
	if(reply.reply_type != AMQP_RESPONSE_NORMAL) {
		cout << " Unable to open the declare exchange " << reply.reply_type << endl;
		return reply.reply_type;
	}

	return 0;

}

int Rabbit::publish(const string& msg) 
{
	if(d_isPublisher) {
		amqp_channel_t channel(1);
		int rc  =  amqp_basic_publish(d_connection,
				channel,
				amqp_cstring_bytes(d_exchangeName.c_str()),
				amqp_cstring_bytes(d_routingKey.c_str()),
				amqp_boolean_t(0),
				amqp_boolean_t(0),
				NULL, 
				amqp_cstring_bytes(msg.c_str()));
	} else {
		cerr << "Not a publisher"<<  endl;
	}


}

int Rabbit::declareQueue() {
	amqp_channel_t channel(1);
	amqp_queue_declare_ok_t *rc  =amqp_queue_declare(d_connection,
			channel,
			amqp_empty_bytes,
			amqp_boolean_t(0),
			amqp_boolean_t(0),
			amqp_boolean_t(1),
			amqp_boolean_t(1),
			amqp_empty_table);

	d_queueName = string( (char*) rc->queue.bytes);
	cout << "Queue name : " << d_queueName << endl;
	return 0;
}

int Rabbit::bindQueue() 
{
    amqp_channel_t channel(1);
	amqp_queue_bind(d_connection,
			channel,
			amqp_cstring_bytes(d_queueName.c_str()),
			amqp_cstring_bytes(d_exchangeName.c_str()),
			amqp_cstring_bytes("arijit"),
			amqp_empty_table);
	return 0;
}

int Rabbit::consume() 
{
	amqp_channel_t channel(1);

	while(true) {
		amqp_basic_consume(d_connection,
				channel,
				amqp_cstring_bytes(d_queueName.c_str()),
				amqp_empty_bytes,
				amqp_boolean_t(0),
				amqp_boolean_t(0),
				amqp_boolean_t(1),

	}
	return 0;

}
