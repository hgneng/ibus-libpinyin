// build command:
// gcc -I/usr/include/speech-dispatcher -lspeechd -o test_speechd.o -c test_speechd.c
// libtool --mode=link gcc -lspeechd test_speechd.o -o test_speechd

#include <libspeechd.h>

int main() {
	/* Open a connection to Speech Dispatcher */
	char *error;
	SPDConnection *conn = spd_open2("IBusSpeech", "main",
			 NULL, SPD_MODE_THREADED, NULL, 1, &error);
	if (conn == NULL) {
		fprintf(stderr, "Failed to connect to Speech Dispatcher:\n%s\n",
			error);
		return 1;
	}

	fprintf(stderr, "spd say success 123");
	spd_say(conn, SPD_TEXT, "spd say success 123");
	return 0;
}