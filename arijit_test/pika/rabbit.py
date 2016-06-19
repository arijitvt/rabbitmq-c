#!/usr/bin/env python
import pika;
import sys;


class Rabbit :
    def __init__(self):
        url = 'amqp://ikpboixq:MNSRLLPQ54loor_B0dJLDKBAwtmgChbx@moose.rmq.cloudamqp.com/ikpboixq';
        self.params = pika.URLParameters(url);
        self.params.socket_timeout = 5;
        return;


    def __connect(self):
        self.connection = pika.BlockingConnection(self.params);
        self.channel = self.connection.channel();
        return;

    def __declareExchange(self):
        self.exchange_name = "arijit";
        self.channel.exchange_declare(exchange=self.exchange_name, type='fanout');
        return ;

    def __declareQueue(self) :
        result = self.channel.queue_declare(exclusive=True);
        self.queue_name = result.method.queue;
        return;

    def __bindQueue(self):
        self.channel.queue_bind(exchange=self.exchange_name,queue=self.queue_name,routing_key=self.exchange_name);
        return;

    def listen(self):
        self.__connect();
        self.__declareExchange();
        self.__declareQueue();
        self.__bindQueue();
        def callback(ch, method, properties, body):
            print(" [x] %r" % body);
            return;
        self.channel.basic_consume(callback,queue=self.queue_name,no_ack= True);
        print "Started lisening";
        self.channel.start_consuming();
        return;

    def publish(self,msg):
        self.__connect();
        self.__declareExchange();
        self.channel.basic_publish(exchange=self.exchange_name, routing_key=self.exchange_name, body=msg)
        print "Published: "+str(msg);


def usage():
    print  "./rabbit.py [pub[msg]|sub]"

def main():
    if len(sys.argv) < 2 : 
        usage();
        return;

    rabbit = Rabbit();

    if sys.argv[1] == "pub" :
        print "Publish"
        rabbit.publish("".join(sys.argv[2:]));

    elif sys.argv[1] == "sub" :
        print "Consuming"
        rabbit.listen();

    else:
        usage();
        return ;
    return;


if __name__ == "__main__":
    main();
