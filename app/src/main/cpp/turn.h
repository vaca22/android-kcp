#pragma once

#ifdef __cplusplus
extern "C" {
#endif
typedef struct {
    char *type;
    char *content;
} Juice_thing;

typedef void SendDataCallback(Juice_thing *thing);


void set_remote_description(char *sdp);
void done_gather();
void add_remote_candidate(char *sdp);
void test_turn(SendDataCallback *sendDataCallback);
#ifdef __cplusplus
}
#endif