#include "header.h"

//#define DEBUG_INFO 1
static const char *TAG = "mpu";

#define I2C_MASTER_SCL_IO           13              /*!<  gpio number D7 for I2C master clock */
#define I2C_MASTER_SDA_IO           12              /*!< gpio number D6 for I2C master data  */
#define I2C_MASTER_NUM             			I2C_NUM_0  		        /*!< I2C port number for master dev */
#define I2C_MASTER_TX_BUF_DISABLE   0               /*!< I2C master do not need buffer */
#define I2C_MASTER_RX_BUF_DISABLE   0               /*!< I2C master do not need buffer */

#define MPU6050_SENSOR_ADDR                 0x68            /*!< slave address for MPU6050 sensor */
#define MPU6050_CMD_START                   0x41            /*!< Command to set measure mode */
#define MPU6050_WHO_AM_I                    0x75            /*!< Command to read WHO_AM_I reg */
#define WRITE_BIT                           I2C_MASTER_WRITE/*!< I2C master write */
#define READ_BIT                            I2C_MASTER_READ /*!< I2C master read */
#define ACK_CHECK_EN                        0x1             /*!< I2C master will check ack from slave*/
#define ACK_CHECK_DIS                       0x0             /*!< I2C master will not check ack from slave */
#define ACK_VAL                             0x0             /*!< I2C ack value */
#define NACK_VAL                            0x1             /*!< I2C nack value */
#define LAST_NACK_VAL                       0x2             /*!< I2C last_nack value */

/**
 * Define the mpu6050 register address:
 */

#define PWR_MGMT_1      0x6B  /*!< Disable sleep mode */
#define WHO_AM_I        0x75  /*!< Command to read WHO_AM_I reg */

#define SMPLRT_DIV      0x19 /*!< Gyro Sample Rate dividing */
#define CONFIG          0x1A /*!< This register configures the external Frame Synchronization (FSYNC) pin sampling and the Digital Low Pass Filter (DLPF) setting for both the gyroscopes and accelerometers */
#define GYRO_CONFIG     0x1B /*!< This register configures Full Scale Range and Self-Test gyroscopes */
#define ACCEL_CONFIG    0x1C /*!< This register configures Full Scale Range and Self-Test accelerometers */


#define GYRO_RANGE		0x08 /* 2000 grad/sec */
#define ACCEL_RANGE		0x09 /* 8g */

#define ACCEL_XOUT_H    0x3B
#define ACCEL_XOUT_L    0x3C
#define ACCEL_YOUT_H    0x3D
#define ACCEL_YOUT_L    0x3E
#define ACCEL_ZOUT_H    0x3F
#define ACCEL_ZOUT_L    0x40

#define TEMP_OUT_H      0x41
#define TEMP_OUT_L      0x42

#define GYRO_XOUT_H     0x43
#define GYRO_XOUT_L     0x44
#define GYRO_YOUT_H     0x45
#define GYRO_YOUT_L     0x46
#define GYRO_ZOUT_H     0x47
#define GYRO_ZOUT_L     0x48

/* date pipline */
QueueHandle_t Qdata_mpu =NULL;

/**
 * @brief i2c master initialization
 */
static esp_err_t i2c_master_init()
{
	int i2c_master_port = I2C_MASTER_NUM;
	i2c_config_t conf;
	conf.mode = I2C_MODE_MASTER;
	conf.sda_io_num = I2C_MASTER_SDA_IO;
	conf.sda_pullup_en = 1;
	conf.scl_io_num = I2C_MASTER_SCL_IO;
	conf.scl_pullup_en = 1;
	conf.clk_stretch_tick = 300; // 300 ticks, Clock stretch is about 210us, you can make changes according to the actual situation.
	ESP_ERROR_CHECK(i2c_driver_install(i2c_master_port, conf.mode));
	ESP_ERROR_CHECK(i2c_param_config(i2c_master_port, &conf));
	return ESP_OK;
}

/**
 * @brief write mpu6050
 *
 * 1. send data
 * ___________________________________________________________________________________________________
 * | start | slave_addr + wr_bit + ack | write reg_address + ack | write data_len byte + ack  | stop |
 * --------|---------------------------|-------------------------|----------------------------|------|
 *
 * @param i2c_num I2C port number
 * @param reg_address slave reg address
 * @param data data to send
 * @param data_len data length
 *
 * @return
 *     - ESP_OK Success
 *     - ESP_ERR_INVALID_ARG Parameter error
 *     - ESP_FAIL Sending command error, slave doesn't ACK the transfer.
 *     - ESP_ERR_INVALID_STATE I2C driver not installed or not in master mode.
 *     - ESP_ERR_TIMEOUT Operation timeout because the bus is busy.
 */
static esp_err_t i2c_master_mpu6050_write(i2c_port_t i2c_num, uint8_t reg_address, uint8_t *data, size_t data_len)
{
	int ret;
	i2c_cmd_handle_t cmd = i2c_cmd_link_create();
	i2c_master_start(cmd);
	i2c_master_write_byte(cmd, MPU6050_SENSOR_ADDR << 1 | WRITE_BIT, ACK_CHECK_EN);
	i2c_master_write_byte(cmd, reg_address, ACK_CHECK_EN);
	i2c_master_write(cmd, data, data_len, ACK_CHECK_EN);
	i2c_master_stop(cmd);
	ret = i2c_master_cmd_begin(i2c_num, cmd, 1000 / portTICK_RATE_MS);
	i2c_cmd_link_delete(cmd);

	return ret;
}

/**
 * @brief code to read mpu6050
 *
 * 1. send reg address
 * ______________________________________________________________________
 * | start | slave_addr + wr_bit + ack | write reg_address + ack | stop |
 * --------|---------------------------|-------------------------|------|
 *
 * 2. read data
 * ___________________________________________________________________________________
 * | start | slave_addr + wr_bit + ack | read data_len byte + ack(last nack)  | stop |
 * --------|---------------------------|--------------------------------------|------|
 *
 * @param i2c_num I2C port number
 * @param reg_address slave reg address
 * @param data data to read
 * @param data_len data length
 *
 * @return
 *     - ESP_OK Success
 *     - ESP_ERR_INVALID_ARG Parameter error
 *     - ESP_FAIL Sending command error, slave doesn't ACK the transfer.
 *     - ESP_ERR_INVALID_STATE I2C driver not installed or not in master mode.
 *     - ESP_ERR_TIMEOUT Operation timeout because the bus is busy.
 */


static esp_err_t i2c_master_mpu6050_read(i2c_port_t i2c_num, uint8_t reg_address, uint8_t *data, size_t data_len)
{
	int ret;
	i2c_cmd_handle_t cmd = i2c_cmd_link_create();
	i2c_master_start(cmd);
	i2c_master_write_byte(cmd, MPU6050_SENSOR_ADDR << 1 | WRITE_BIT, ACK_CHECK_EN);
	i2c_master_write_byte(cmd, reg_address, ACK_CHECK_EN);
	i2c_master_stop(cmd);
	ret = i2c_master_cmd_begin(i2c_num, cmd, 1000 / portTICK_RATE_MS);
	i2c_cmd_link_delete(cmd);

	if (ret != ESP_OK) {
		return ret;
	}

	cmd = i2c_cmd_link_create();
	i2c_master_start(cmd);
	i2c_master_write_byte(cmd, MPU6050_SENSOR_ADDR << 1 | READ_BIT, ACK_CHECK_EN);
	i2c_master_read(cmd, data, data_len, LAST_NACK_VAL);
	i2c_master_stop(cmd);
	ret = i2c_master_cmd_begin(i2c_num, cmd, 1000 / portTICK_RATE_MS);
	i2c_cmd_link_delete(cmd);

	return ret;
}

static esp_err_t i2c_master_mpu6050_init(i2c_port_t i2c_num)
{
	uint8_t cmd_data;
	vTaskDelay(100 / portTICK_RATE_MS);
	i2c_master_init();  // init i2c proto
	cmd_data = 0x00;    // reset mpu6050
	ESP_ERROR_CHECK(i2c_master_mpu6050_write(i2c_num, PWR_MGMT_1, &cmd_data, 1));
	cmd_data = 0x07;    // Set the SMPRT_DIV
	ESP_ERROR_CHECK(i2c_master_mpu6050_write(i2c_num, SMPLRT_DIV, &cmd_data, 1));
	cmd_data = 0x06;    // Set the Low Pass Filter set
	ESP_ERROR_CHECK(i2c_master_mpu6050_write(i2c_num, CONFIG, &cmd_data, 1));
	cmd_data = GYRO_RANGE;    // Set the GYRO range
	ESP_ERROR_CHECK(i2c_master_mpu6050_write(i2c_num, GYRO_CONFIG, &cmd_data, 1));
	cmd_data = ACCEL_RANGE;    // Set the ACCEL range
	ESP_ERROR_CHECK(i2c_master_mpu6050_write(i2c_num, ACCEL_CONFIG, &cmd_data, 1));
	return ESP_OK;
}

esp_err_t mpu_get_accel_data(struct date_i2c *date){
	int ret;
	uint8_t data_int[6];

	memset(data_int, 0, 6);
	ret = i2c_master_mpu6050_read(I2C_MASTER_NUM, ACCEL_XOUT_H, data_int, 6);
	date->x=(int16_t)((data_int[0] << 8) | data_int[1]);
	date->y=(int16_t)((data_int[2] << 8) | data_int[3]);
	date->z=(int16_t)((data_int[4] << 8) | data_int[5]);
#ifdef DEBUG_INFO
	ESP_LOGI(TAG,"sensor accelerometers_X %d\n",(int16_t)((data_int[0] << 8) | data_int[1]));
	ESP_LOGI(TAG,"sensor accelerometers_Y %d\n",(int16_t)((data_int[2] << 8) | data_int[3]));
	ESP_LOGI(TAG,"sensor accelerometers_Z %d\n",(int16_t)((data_int[4] << 8) | data_int[5]));
#endif
	return ret;

}

esp_err_t mpu_get_gyro_data(struct date_i2c *date){
	int ret;
	uint8_t data_int[6];

	memset(data_int, 0, 6);
	ret = i2c_master_mpu6050_read(I2C_MASTER_NUM, GYRO_XOUT_H, data_int, 6);
	date->x=(int16_t)((data_int[0] << 8) | data_int[1]);
	date->y=(int16_t)((data_int[2] << 8) | data_int[3]);
	date->z=(int16_t)((data_int[4] << 8) | data_int[5]);
#ifdef DEBUG_INFO
	ESP_LOGI(TAG,"sensor gyroscopes X:%d\n",(int16_t)((data_int[0] << 8) | data_int[1]));
	ESP_LOGI(TAG,"sensor gyroscopes Y:%d\n",(int16_t)((data_int[2] << 8) | data_int[3]));
	ESP_LOGI(TAG,"sensor gyroscopes Z:%d\n",(int16_t)((data_int[4] << 8) | data_int[5]));
#endif

	return ret;

}

esp_err_t mpu_get_temp_data(uint16_t *temp){
	int ret;
	uint8_t data_int[2];

	memset(data_int, 0, 2);
	ret = i2c_master_mpu6050_read(I2C_MASTER_NUM, TEMP_OUT_H, data_int, 2);
	*temp=36.53 + ((double)(int16_t)((data_int[0] << 8) | data_int[2]) / 340);

#ifdef DEBUG_INFO
	ESP_LOGI(TAG, "TEMP: %d.%d\n", (uint16_t)*temp, (uint16_t)(*temp * 100) % 100);
#endif

	return ret;

}

size_t json_serilizete(struct date_i2c *date,char *str){
	size_t len=0;
	len=sprintf (str,"{ \"name\": \"%-1s\", \"X\": %i, \"Y\": %i, \"Z\": %i }\n", "acel",date->x,date->y,date->z);
	str[len]='\0';
	return len;
}

void i2c_task_display(void *arg)
{
	struct date_i2c accel_D;
	struct date_i2c gyro_D;

	struct date_i2c accel_D_int;
	struct date_i2c gyro_D_int;


	uint8_t who_am_i;
	int ret[2];
	memset(ret,0,2);


	do{

		i2c_master_mpu6050_read(I2C_MASTER_NUM, WHO_AM_I, &who_am_i, 1);

		if (0x68 != who_am_i) {
			ESP_LOGW("who_am_i != 0x68 ,who_am_i %x",who_am_i);
		}

	}while(0x68 != who_am_i);

	mpu_get_accel_data(&accel_D_int);
	mpu_get_gyro_data(&gyro_D_int);

	while (1) {
		who_am_i = 0;
		ret[0] = mpu_get_accel_data(&accel_D);
		ret[1] = mpu_get_gyro_data(&gyro_D);

		if ((ret[0]&ret[1]) == ESP_OK) {
			/* Send the entire structure to the queue created to hold 10 structures. */
			xQueueSend( /* The handle of the queue. */
					Qdata_mpu,
					/* The address of the xMessage variable.  sizeof( struct AMessage )
                        bytes are copied from here into the queue. */
					( void * ) &accel_D,
					/* Block time of 0 says don't block if the queue is already full.
                        Check the value returned by xQueueSend() to know if the message
                        was sent to the queue successfully. */
					( TickType_t ) 10/portTICK_RATE_MS );
		} else {
			ESP_LOGE(TAG, "No ack, sensor not connected...skip...\n");
		}

		vTaskDelay(100 / portTICK_RATE_MS);
	}

	i2c_driver_delete(I2C_MASTER_NUM);
	vTaskDelete(NULL);
}

size_t mpu_get_date(char *json_str){
	struct date_i2c date;
	size_t len=0;

	len=json_serilizete(&date,json_str);
	while(Qdata_mpu == NULL){};
	if( xQueueReceive( Qdata_mpu,
			&( date ),
			portMAX_DELAY ) == pdPASS)
	{
		len=json_serilizete(&date,json_str);
	}
	return len;
}

esp_err_t mpu_init(){
	i2c_master_mpu6050_init(I2C_MASTER_NUM);
	Qdata_mpu = xQueueCreate(4,sizeof(Date_ini ));
	return ESP_OK;
}

esp_err_t mpu_start(uint8_t prior,uint16_t mem_us){
	xTaskCreate(i2c_task_display, "mpu_monitor", mem_us, NULL, prior, &server_mpu_task_hdl);

	return ESP_OK;
}
