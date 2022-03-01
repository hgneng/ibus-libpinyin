#include <libspeechd.h>
#include <stdio.h>
#include <string.h>
#include <glib.h>
#include "utf8.h"

using namespace std;

extern int ibs_word_begin;
extern int ibs_word_end;
extern char* ibs_words[];
int ibs_char_limit;
bool g_is_mute = true;

static SPDConnection* g_spd = 0;
static char g_result[1024];

void ibs_destroy() {
  if (g_spd)
    spd_close(g_spd);
}

void ibs_speak(const char *text) {
  g_message("ibs_speak: %s", text);
  if (g_is_mute) {
    return;
  }
  //spd_cancel(g_spd);
  int ret = spd_say(g_spd, SPD_TEXT, text);
  //g_message("ibs_speak: ret=%d", ret);
}

void ibs_speak_politely(const char *text) {
  g_message("ibs_speak_politely:%s", text);
  if (g_is_mute) {
    return;
  }
  int ret = spd_say(g_spd, SPD_MESSAGE, text);
  //g_message("ibs_speak: ret=%d", ret);
}

string ibs_get_utf8(int code) {
  char buf[5] = {0, 0, 0, 0, 0};
  try {
    utf8::append(code, buf);
  } catch (...) {
    //cerr << "code point:" << code << endl;
  }
  return string(buf);
};

extern "C"
void ibs_explain(char *text) {
  g_message("ibs_explain: %s", text);

  if (strlen(text) / 3 <= ibs_char_limit) {
    string s(text);
    string result;
    std::string::iterator it = s.begin();
    int code = 0;
    bool first = true;
    const char *c = 0;
    while (it != s.end()) {
      code = utf8::next(it, s.end());
      g_message("ibs_explain: %d", code);
      if (code < ibs_word_end && code > 0 && ibs_words[code] != NULL) {
        c = ibs_words[code];
      } else {
        c = ibs_get_utf8(code).c_str();
      }

      result.append(c);
    }
    ibs_speak(result.c_str());
  } else {
    ibs_speak(text);
  }
}

void ibs_exec(const char *cmd) {
  FILE *fp;

  /* Open the command for reading. */
  fp = popen(cmd, "r");
  if (fp == NULL) {
    printf("Failed to run %s\n", cmd);
    return;
  }

  g_result[0] = 0;
  char *ret = fgets(g_result, sizeof(g_result) - 1, fp);
  g_message("ibs_exec: %s", g_result);

  pclose(fp);
}

bool ibs_is_proc_running(const char *cmd) {
  char buffer[1024];
  snprintf(buffer, 1023, "ps -ef|grep '%s'|grep -v grep", cmd);
  ibs_exec(buffer);
  if (strlen(g_result) > 0)
    return true;
  else
    return false;
}

void ibs_stop() {
  //printf("ibs_stop\n");
  spd_cancel(g_spd);
  g_is_mute = !ibs_is_proc_running("orca");
}

/* rate in Orac is 0-100, default is 50 */
int ibs_get_orca_rate() {
  ibs_exec("grep rate ~/.local/share/orca/user-settings.conf | awk '{print $2}'");
  if (strlen(g_result) > 0) {
    int rate = atoi(g_result);
    if (rate < 0 || rate > 100)
      rate = 50;
    return rate;
  } else {
    return 50;
  }
}

int ibs_get_config_rate() {
  ibs_exec("grep rate ~/.dog/config/ibusreader | awk -F= '{print $2}' 2>/dev/null");
  if (strlen(g_result) > 0) {
    int rate = atoi(g_result);
    if (rate < 0 || rate > 100)
      rate = -1;
    return rate;
  } else {
    return -1;
  }
}

void ibs_update_rate() {
  int rate = ibs_get_config_rate();
  if (rate == -1)
    rate = ibs_get_orca_rate();
  spd_set_voice_rate(g_spd, (int)((rate - 50) * 2));
  g_message("ibs_update_rate: %d -> %d", rate, (int)((rate - 50) * 2));
}

void ibs_update_char_limit() {
  ibs_exec("grep char_limit ~/.dog/config/ibusreader | awk -F= '{print $2}' 2>/dev/null");
  ibs_char_limit = 2;
  if (strlen(g_result) > 0) {
    int char_limit = atoi(g_result);
    if (char_limit > 1) {
      ibs_char_limit = char_limit;
    }
  }
  g_message("ibs_update_char_limit: %d", ibs_char_limit);
}

extern "C"
void ibs_init() {
  g_message("ibs_init");
  if (!g_spd) {
    char *error_result = (char*)malloc(1024);
    g_spd = spd_open2("IBusSpeech", "main", NULL, SPD_MODE_THREADED, NULL, 1, &error_result);
    if (g_spd) {
      spd_set_output_module(g_spd, "ekho");
      spd_set_language(g_spd, "Mandarin");
      ibs_update_rate();
      ibs_update_char_limit();
      if (ibs_is_proc_running("orca")) {
        g_is_mute = false;
        spd_say(g_spd, SPD_TEXT, "i-bus reader 已启动");
      }
    } else {
      g_message("fail to init speechd: %s", error_result);
    }
  }
  g_message("ibs_init end");
}
/*
int main() {
  printf("begin\n");
  ibs_init();
  printf("end\n");
}*/
