Download wt first. Then, type "make" inside the folder to compile. 


Then, type 

	./hey --docroot . --http-listen 0.0.0.0:8080

When running on GAUL, 8080 should be changed to some other available port. 


When running the program on gaul and you are NOT connected to UWO network, type 

	ssh -N -L 8080:cs3307.gaul.csd.uwo.ca:8080 cs3307.gaul.csd.uwo.ca

on your LOCAL machine. Change 8080 to the same port number you picked earlier. 

For now, log in with "user" and "password" for username and password. 