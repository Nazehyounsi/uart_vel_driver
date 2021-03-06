// C library headers
#include <stdio.h>

// Linux headers
#include <fcntl.h> // Contains file controls like O_RDWR
#include <errno.h> // Error integer and strerror() function
#include <termios.h> // Contains POSIX terminal control definitions
#include <unistd.h> // write(), read(), close()
#include <string>

#include <memory>
#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/twist.hpp"

using std::placeholders::_1;


using namespace geometry_msgs::msg;

class UartVelSub : public rclcpp::Node
{
public :

  UartVelSub() : Node("uart_vel_sub")    
  {
    subscription_= this->create_subscription<Twist>( "cmd_vel", 1000,std::bind(&UartVelSub::vel_cmd_callback, this, _1));
  }

  
private :
  
  void vel_cmd_callback(const Twist::SharedPtr data ) const
  {
    RCLCPP_INFO(this->get_logger(),"velocity message {x= %s, y= %s , z= %s  ",data->linear.x,data->linear.y,data->linear.z);

    //convert de vel data to unsigned char*  to send them to the serialport
     std::string x_vel = "x = " + std::to_string(data->linear.x);
     std::string y_vel = "y = " + std::to_string(data->linear.y);
     std::string z_vel = "z = " + std::to_string(data->linear.z);
     unsigned char* x_tab =(unsigned char*)x_vel.c_str();
     unsigned char* y_tab =(unsigned char*)y_vel.c_str();
     unsigned char* z_tab =(unsigned char*)z_vel.c_str();
    
    // Open the serial port. Change device path as needed (currently set to an standard FTDI USB-UART cable type device)
    int serial_port = open("/dev/ttyS0", O_RDWR);
    // Write to serial port
    write(serial_port,x_tab,sizeof(x_tab));
    write(serial_port,y_tab,sizeof(y_tab));
    write(serial_port,z_tab,sizeof(z_tab));
}
  rclcpp::Subscription<geometry_msgs::msg::Twist>::SharedPtr subscription_;
};


  // Open the serial port. Change device path as needed (currently set to an standard FTDI USB-UART cable type device)
    int serial_port = open("/dev/ttyUSB0", O_RDWR);
    


int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<UartVelSub>());
  rclcpp::shutdown();
  return 0;


  // Create new termios struc, we call it 'tty' for convention
struct termios tty;

 
  // Read in existing settings, and handle any error
if (tcgetattr(serial_port, &tty) != 0) {
  printf("Error %i from tcgetattr: %s\n", errno, strerror(errno));
}

tty.c_cflag &= ~PARENB; // Clear parity bit, disabling parity (most common)
tty.c_cflag &= ~CSTOPB; // Clear stop field, only one stop bit used in communication (most common)
tty.c_cflag |= CS8; // 8 bits per byte (most common)
tty.c_cflag &= ~CRTSCTS; // Disable RTS/CTS hardware flow control (most common)
tty.c_cflag |= CREAD | CLOCAL; // Turn on READ & ignore ctrl lines (CLOCAL = 1)

tty.c_lflag &= ~ICANON;
tty.c_lflag &= ~ECHO; // Disable echo
tty.c_lflag &= ~ECHOE; // Disable erasure
tty.c_lflag &= ~ECHONL; // Disable new-line echo
tty.c_lflag &= ~ISIG; // Disable interpretation of INTR, QUIT and SUSP
tty.c_iflag &= ~(IXON | IXOFF | IXANY); // Turn off s/w flow ctrl
tty.c_iflag &= ~(IGNBRK|BRKINT|PARMRK|ISTRIP|INLCR|IGNCR|ICRNL); // Disable any special handling of received bytes

tty.c_oflag &= ~OPOST; // Prevent special interpretation of output bytes (e.g. newline chars)
tty.c_oflag &= ~ONLCR; // Prevent conversion of newline to carriage return/line feed
// tty.c_oflag &= ~OXTABS; // Prevent conversion of tabs to spaces (NOT PRESENT ON LINUX)
// tty.c_oflag &= ~ONOEOT; // Prevent removal of C-d chars (0x004) in output (NOT PRESENT ON LINUX)

tty.c_cc[VTIME] = 10;    // Wait for up to 1s (10 deciseconds), returning as soon as any data is received.
tty.c_cc[VMIN] = 0;

// Set in/out baud rate to be 9600
cfsetispeed(&tty, B9600);
cfsetospeed(&tty, B9600);

// Save tty settings, also checking for error
if (tcsetattr(serial_port, TCSANOW, &tty) != 0) {
    printf("Error %i from tcsetattr: %s\n", errno, strerror(errno));
}

//test if it works 
 unsigned char msg[] = { 'H', 'e', 'l', 'l', 'o', '\r' };
 write(serial_port, "Hello, world!", sizeof(msg));
 
// Allocate memory for read buffer, set size according to your needs
char read_buf [256];

// Normally you wouldn't do this memset() call, but since we will just receive
// ASCII data for this example, we'll set everything to 0 so we can
// call printf() easily.
 memset(&read_buf, '\0', sizeof(read_buf));

// Read bytes. The behaviour of read() (e.g. does it block?,
// how long does it block for?) depends on the configuration
// settings above, specifically VMIN and VTIME
int num_bytes = read(serial_port, &read_buf, sizeof(read_buf));

// n is the number of bytes read. n may be 0 if no bytes were received, and can also be -1 to signal an error.
if (num_bytes < 0) {
   printf("Error reading: %s", strerror(errno));
}

// Here we assume we received ASCII data, but you might be sending raw bytes (in that case, don't try and
// print it to the screen like this!)
printf("Read %i bytes. Received message: %s", num_bytes, read_buf);

 close(serial_port);


       }
  
