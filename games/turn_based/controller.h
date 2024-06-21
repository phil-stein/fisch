#ifndef TURN_BASED_CONTROLLER_H
#define TURN_BASED_CONTROLLER_H

#ifdef __cplusplus
extern "C" {
#endif


void controller_init();
void controller_update();
void controller_cleanup();

#ifdef __cplusplus
} // extern C
#endif


#endif  // TURN_BASED_CONTROLLER_H
