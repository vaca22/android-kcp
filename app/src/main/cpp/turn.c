/**
 * Copyright (c) 2020 Paul-Louis Ageneau
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "juice.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "turn.h"

#include <unistd.h> // for sleep
#include <malloc.h>



#define BUFFER_SIZE 4096

static juice_agent_t *agent1;


static void on_state_changed1(juice_agent_t *agent, juice_state_t state, void *user_ptr);

static void on_candidate1(juice_agent_t *agent, const char *sdp, void *user_ptr);

static void on_gathering_done1(juice_agent_t *agent, void *user_ptr);

static void on_recv1(juice_agent_t *agent, const char *data, size_t size, void *user_ptr);

SendDataCallback *mySendDataCallback=NULL;


void set_remote_description(char *sdp){
    juice_set_remote_description(agent1, sdp);
    char sdp1[JUICE_MAX_SDP_STRING_LEN];
    juice_get_local_description(agent1, sdp1, JUICE_MAX_SDP_STRING_LEN);
    Juice_thing *thing=malloc(sizeof(Juice_thing));
    thing->type="ice";
    thing->content= malloc(strlen(sdp1)+1);
    strcpy(thing->content,sdp1);
    if(mySendDataCallback){
        mySendDataCallback(thing);
    }
}

void add_remote_candidate(char *sdp){
    juice_add_remote_candidate(agent1, sdp);
    juice_gather_candidates(agent1);
}

void done_gather(){
    printf("fuck");
    juice_set_remote_gathering_done(agent1);
}

void test_turn(SendDataCallback *sendDataCallback) {

    mySendDataCallback=sendDataCallback;
	juice_set_log_level(JUICE_LOG_LEVEL_DEBUG);

	// Agent 1: Create agent
	juice_config_t config1;
	memset(&config1, 0, sizeof(config1));

	// STUN server example (use your own server in production)
	config1.stun_server_host = "vaca.vip";
	config1.stun_server_port = 9669;

	// TURN server example (use your own server in production)
	juice_turn_server_t turn_server;
	memset(&turn_server, 0, sizeof(turn_server));
	turn_server.host = "vaca.vip";
	turn_server.port = 9669;
	turn_server.username = "vaca";
	turn_server.password = "123456";
	config1.turn_servers = &turn_server;
	config1.turn_servers_count = 1;

	config1.cb_state_changed = on_state_changed1;
	config1.cb_candidate = on_candidate1;
	config1.cb_gathering_done = on_gathering_done1;
	config1.cb_recv = on_recv1;
	config1.user_ptr = NULL;

	agent1 = juice_create(&config1);





	// Agent 1: Generate local description









//	juice_set_remote_description(agent1, sdp2);

	// Agent 1: Gather candidates (and send them to agent 2)
//	juice_gather_candidates(agent1);



	// -- Connection should be finished --

	// Check states
//	juice_state_t state1 = juice_get_state(agent1);

//	bool success = ((state1 == JUICE_STATE_COMPLETED || state1 == JUICE_STATE_CONNECTED));
//
//	// Retrieve candidates
//	char local[JUICE_MAX_CANDIDATE_SDP_STRING_LEN];
//	char remote[JUICE_MAX_CANDIDATE_SDP_STRING_LEN];
//	if (success &=
//	    (juice_get_selected_candidates(agent1, local, JUICE_MAX_CANDIDATE_SDP_STRING_LEN, remote,
//	                                   JUICE_MAX_CANDIDATE_SDP_STRING_LEN) == 0)) {
//		printf("Local candidate  1: %s\n", local);
//		printf("Remote candidate 1: %s\n", remote);
//
//		success &= (strstr(local, "relay") != NULL);
//	}
//
//
//	// Retrieve addresses
//	char localAddr[JUICE_MAX_ADDRESS_STRING_LEN];
//	char remoteAddr[JUICE_MAX_ADDRESS_STRING_LEN];
//	if (success &= (juice_get_selected_addresses(agent1, localAddr, JUICE_MAX_ADDRESS_STRING_LEN,
//	                                             remoteAddr, JUICE_MAX_ADDRESS_STRING_LEN) == 0)) {
//		printf("Local address  1: %s\n", localAddr);
//		printf("Remote address 1: %s\n", remoteAddr);
//	}


	// Agent 1: destroy
//	juice_destroy(agent1);



//	if (success) {
//		printf("Success\n");
//		return 0;
//	} else {
//		printf("Fauck\n");
//		return 0;
//	}
}

// Agent 1: on state changed
static void on_state_changed1(juice_agent_t *agent, juice_state_t state, void *user_ptr) {
	printf("State 1: %s\n", juice_state_to_string(state));

	if (state == JUICE_STATE_CONNECTED) {
		// Agent 1: on connected, send a message
		const char *message = "Hello from 2";
		juice_send(agent, message, strlen(message));
	}
}


// Agent 1: on local candidate gathered
static void on_candidate1(juice_agent_t *agent, const char *sdp, void *user_ptr) {
	// Filter relayed candidates
	if (!strstr(sdp, "relay"))
		return;

	printf("Candidate 1: %s\n", sdp);
    Juice_thing *thing=malloc(sizeof(Juice_thing));
    thing->type="candidate";
    thing->content= malloc(strlen(sdp)+1);
    strcpy(thing->content,sdp);
    if(mySendDataCallback){
        mySendDataCallback(thing);
    }

	// Agent 2: Receive it from agent 1
//	juice_add_remote_candidate(agent2, sdp);
}



// Agent 1: on local candidates gathering done
static void on_gathering_done1(juice_agent_t *agent, void *user_ptr) {
	printf("Gathering done 1\n");
    Juice_thing *thing=malloc(sizeof(Juice_thing));
    thing->type="done";
    thing->content="done";
    if(mySendDataCallback){
        mySendDataCallback(thing);
    }
}



// Agent 1: on message received
static void on_recv1(juice_agent_t *agent, const char *data, size_t size, void *user_ptr) {
	char buffer[BUFFER_SIZE];
	if (size > BUFFER_SIZE - 1)
		size = BUFFER_SIZE - 1;
	memcpy(buffer, data, size);
	buffer[size] = '\0';
	printf("fuckyou : %s\n", buffer);
}

