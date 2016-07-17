#include <rabbit.h>



int main (int argc, char **argv) {
                                                                        
	Rabbit rabbit("moose.rmq.cloudamqp.com","ikpboixq","ikpboixq","MNSRLLPQ54loor_B0dJLDKBAwtmgChbx",5672);
	string input;                             

	string arg(argv[1]); 

	if(arg == "pub") {
		rabbit.initPublisher();

		while(true) {
			cout << "Enter message to publish" << endl;
			cin >> input ; 
			if(input == "stop") {
				break;
			}
			rabbit.publish(input);
		}

	} else if ( arg == "sub" ) 
	{
		rabbit.initConsumer();
	
	}
	
	return 0;
}
