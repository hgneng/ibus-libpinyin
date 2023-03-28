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
	int ret = spd_set_output_module(conn, "voxin");
	fprintf(stderr, "spd_set_output_module: %d\n", ret);

	fprintf(stderr, "rate: %d\n", spd_get_voice_rate(conn));
	fprintf(stderr, "spd say success 123\n");
	spd_say(conn, SPD_MESSAGE, "spd say success 123");
	spd_set_voice_rate(conn, 34);
	fprintf(stderr, "rate: %d\n", spd_get_voice_rate(conn));
	fprintf(stderr, "spd say success 123\n");
	spd_say(conn, SPD_MESSAGE, "spd say success 123");
	spd_set_voice_rate(conn, -30);
	fprintf(stderr, "rate: %d\n", spd_get_voice_rate(conn));
	fprintf(stderr, "spd say success 456\n");
	spd_say(conn, SPD_MESSAGE, "spd say success 456");
	return 0;
}
