
#define EVENT_1S (1 << 0)
#define EVENT_100MS (1 << 1)
#define EVENT_BOOT (1 << 2)

extern volatile int gEvent;

extern int gTimer100ms;
extern int gTimer1s;
extern int gTimer1sBrewing;

void setup_timer();
void background_event();