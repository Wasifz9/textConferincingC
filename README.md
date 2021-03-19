# Text Conf

## status
it accepts a user on login then enters that users response loop which never returns. 
so it cant accept more than one user right now.

login and create session work though and server is keeping track of clients and sessions correctly 

## to do 
multi threading the server to accept new clients while serveing requests for old clients 
a) im thinking use pthread to assign new threads to a client whenever they join 

writing out the functions for the server and client 
a) writing leave session can be done before multi threading (might do that rn)