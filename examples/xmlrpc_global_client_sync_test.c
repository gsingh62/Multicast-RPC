/* A simple synchronous XML-RPC client program written in C, as an example of
   an Xmlrpc-c client.  This invokes the sample.add procedure that the
   Xmlrpc-c example xmlrpc_sample_add_server.c server provides.  I.e. it adds
   two numbers together, the hard way.

   This sends the RPC to the server running on the local system ("localhost"),
   HTTP Port 8080.
*/

#include <stdlib.h>
#include <stdio.h>

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



int 
main(int           const argc, 
     const char ** const argv) {

    xmlrpc_env env;
    xmlrpc_value * resultP,*sumP;
    xmlrpc_int sum;
    int numServers = 3, i;  
    const char * const any_or_majority = "any";
    const char * const methodName = "sample.add";

    if (argc-1 > 0) {
        fprintf(stderr, "This program has no arguments\n");
        exit(1);
    }

    /* Initialize our error-handling environment. */
    xmlrpc_env_init(&env);

    /* Start up our XML-RPC client library. */
    xmlrpc_client_init2(&env, XMLRPC_CLIENT_NO_FLAGS, NAME, VERSION, NULL, 0);
    dieIfFaultOccurred(&env);
    printf("xmlrpc initialized\n");
    /* Initialize our error-handling environment. */
    xmlrpc_env_init(&env);
    char * serverUrl1 = "http://10.0.2.15:8080/RPC2";
	printf("Making XMLRPC call to server url '%s' method '%s' "
           "to request the sum "
           "of 5 and 7...\n", serverUrl1, methodName);
    char * serverUrl2 = "http://10.0.2.15:9090/RPC2";
	printf("Making XMLRPC call to server url '%s' method '%s' "
           "to request the sum "
           "of 5 and 7...\n", serverUrl2, methodName);
    char * serverUrl3 = "http://10.0.2.15:4080/RPC2";
	printf("Making XMLRPC call to server url '%s' method '%s' "
           "to request the sum "
           "of 5 and 7...\n", serverUrl3, methodName);

    /* Make the remote procedure call */
    resultP =  xmlrpc_client_call50(numServers,&env,methodName,"(ii)",(xmlrpc_int32) 10, (xmlrpc_int32) 8,any_or_majority,serverUrl1,serverUrl2,serverUrl3);
    dieIfFaultOccurred(&env);
    printf("All RPCs finished\n");
    /* Get our sum and print it out. */
    int array_len = xmlrpc_array_size(&env,resultP);
    printf("return len %d\n",array_len);
    for(i=0;i<array_len;i++)
    {
    xmlrpc_array_read_item(&env,resultP,i,&sumP);
    xmlrpc_read_int(&env, sumP, &sum);
    printf("The sum is %d\n", sum);
    dieIfFaultOccurred(&env);
    }
    
    /* Dispose of our result value. */
    xmlrpc_DECREF(resultP);

    /* Clean up our error-handling environment. */
    xmlrpc_env_clean(&env);
    
    /* Shutdown our XML-RPC client library. */
    xmlrpc_client_cleanup();

    return 0;
}