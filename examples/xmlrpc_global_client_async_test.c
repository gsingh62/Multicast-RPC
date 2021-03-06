/* A simple synchronous XML-RPC client program written in C, as an example of
   an Xmlrpc-c client.  This invokes the sample.add procedure that the
   Xmlrpc-c example xmlrpc_sample_add_server.c server provides.  I.e. it adds
   two numbers together, the hard way.

   This sends the RPC to the server running on the local system ("localhost"),
   HTTP Port 8080.
*/

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>

#include <xmlrpc-c/base.h>
#include <xmlrpc-c/client.h>
#include<xmlrpc-c/client_global.h>
#include "config.h"  /* information about this build environment */

#define NAME "Xmlrpc-c Test Client"
#define VERSION "1.0"





static void 
dieIfFaultOccurred (xmlrpc_env * const envP) {
    if (envP->fault_occurred) {
        fprintf(stderr, "ERROR: %s (%d)\n",
                envP->fault_string, envP->fault_code);
        exit(1);
    }
}

static void 
handle_sample_add_response(const char *   const serverUrl,
                           const char *   const methodName,
                           xmlrpc_value * const paramArrayP,
                           void *         const user_data,
                           xmlrpc_env *   const faultP,
                           xmlrpc_value * const resultP) {

    xmlrpc_env env;
    xmlrpc_int addend, adder;
    
    /* Initialize our error environment variable */
    xmlrpc_env_init(&env);

    /* Our first four arguments provide helpful context.  Let's grab the
       addends from our parameter array. 
    */
    xmlrpc_decompose_value(&env, paramArrayP, "(ii)", &addend, &adder);
    dieIfFaultOccurred(&env);

    printf("RPC with method '%s' at URL '%s' to add %d and %d "
           "has completed\n", methodName, serverUrl, addend, adder);
    
    if (faultP->fault_occurred)
        printf("The RPC failed.  %s\n", faultP->fault_string);
    else {
        xmlrpc_int sum;

        xmlrpc_read_int(&env, resultP, &sum);
        dieIfFaultOccurred(&env);

        printf("The sum is  %d\n", sum);
    }
}

void homeFunc(){
	sleep(10);
}

int 
main(int           const argc, 
     const char ** const argv) {

    clock_t t;
    xmlrpc_env env;
    xmlrpc_value * resultP;
    xmlrpc_int32 sum; 
    int i;
    int numServers = 3;  
    const char * const any_or_majority = argv[1];
    printf("any or majority is %s\n",argv[1]);
    const char * const methodName = "sample.add";
    printf("variables declared\n");
   /* if (argc-1 > 0) {
        fprintf(stderr, "This program has no arguments\n");
        exit(1);
    }*/

     /* Initialize our error-handling environment. */
    xmlrpc_env_init(&env);

    /* Start up our XML-RPC client library. */
    xmlrpc_client_init2(&env, XMLRPC_CLIENT_NO_FLAGS, NAME, VERSION, NULL, 0);
    dieIfFaultOccurred(&env);
    printf("xmlrpc initialized\n");
    /* Initialize our error-handling environment. */
    xmlrpc_env_init(&env);
	//10.0.2.15
    char * serverUrl1 = "http://localhost:8080/RPC2";
	printf("Making XMLRPC call to server url '%s' method '%s' "
           "to request the sum "
           "of 8 and 1...\n", serverUrl1, methodName);
    char * serverUrl2 = "http://localhost:9090/RPC2";
	printf("Making XMLRPC call to server url '%s' method '%s' "
           "to request the sum "
           "of 8 and 1...\n", serverUrl2, methodName);
    char * serverUrl3 = "http://localhost:4080/RPC2";
	printf("Making XMLRPC call to server url '%s' method '%s' "
           "to request the sum "
           "of 8 and 1...\n", serverUrl3, methodName);
    	/* Make the remote procedure call */
	t=clock();
xmlrpc_client_call_asynch(numServers,methodName,handle_sample_add_response,NULL,"(ii)", (xmlrpc_int32) 8, (xmlrpc_int32) 1,any_or_majority,serverUrl1,serverUrl2,serverUrl3);
    	dieIfFaultOccurred(&env);
	t = clock()-t;
	homeFunc();
	printf("RPCs all requested. Waiting for & handling respones...\n");
	xmlrpc_client_event_loop_finish_asynch();
	printf("All RPCs finished");
	
	 /* Get our sum and print it out. */
    	//xmlrpc_read_int(&env, resultP, &sum);
    	dieIfFaultOccurred(&env);
	printf("It took me %d clicks (%f seconds).\n",t,((float) t)/CLOCKS_PER_SEC);
   	//printf("The sum is %d\n", sum);
    	/* Dispose of our result value. */
    	xmlrpc_DECREF(resultP);
    

    /* Clean up our error-handling environment. */
    xmlrpc_env_clean(&env);
    
    /* Shutdown our XML-RPC client library. */
    xmlrpc_client_cleanup();

    return 0;
}
