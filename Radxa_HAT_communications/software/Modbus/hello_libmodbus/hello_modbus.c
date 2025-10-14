#include <unistd.h>
#include <stdio.h>
#include <stdint.h>
#include <errno.h>
#include "modbus.h"

#define SERVER_IP "192.168.0.52"
#define TAB_REG_NUM 10
#define START_REG 0
#define READ_REG_NUM TAB_REG_NUM

/* Push bottons */
#define FWD_REV 1
#define RUN 2
#define STOP 3

void read_server(modbus_t *mb);
void push_btn(modbus_t *mb, uint8_t reg);

int main(void) {
  /* Init coommunication */
  modbus_t *mb = modbus_new_tcp(SERVER_IP, MODBUS_TCP_DEFAULT_PORT);
  if (mb == NULL){
    fprintf(stderr, "Unable to allocate libmodbus context\n");
    return -1;
  }
  if (modbus_connect(mb) == -1){
    fprintf(stderr, "Connection failed: %s\n", modbus_strerror(errno));
    modbus_free(mb);
    return -1;
  }
  
  for (int i=0; i < 3; i++) {
    push_btn(mb, FWD_REV);
    push_btn(mb, STOP);
    sleep(1);
    push_btn(mb, RUN);
    sleep(1);
  }

  for (int i = 0; i < 6001; i = i + 50) {
    modbus_write_register(mb, 0, i);
  }
  
  // Print PLC server
  read_server(mb);

  modbus_close(mb);
  modbus_free(mb);  

  return  0;
}

void read_server(modbus_t *mb){
  /* Read READ_REG_NUM registers from the address START_REG */
  uint16_t tab_reg[TAB_REG_NUM] = {0};
  int rc = modbus_read_registers(mb, START_REG, READ_REG_NUM, tab_reg);
  if (rc == -1){
    fprintf(stderr, "%s\n", modbus_strerror(errno));
  }else{
    for (int i=0; i < rc; i++) printf("reg[%d]=%d (0x%X)\n", i, tab_reg[i], tab_reg[i]);
  }
}

void push_btn(modbus_t *mb, uint8_t reg){
  modbus_write_register(mb, reg, 1);
  modbus_write_register(mb, reg, 0);
}
