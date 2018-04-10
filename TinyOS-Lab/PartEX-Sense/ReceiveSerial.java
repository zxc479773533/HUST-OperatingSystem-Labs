/*
 * HUST IOT TinyOS Lab Part EX - Sense
 *
 * By Pan Yue, modifyed from tinyos/apps/test/TestSerial.java
 */

import java.io.IOException;

import net.tinyos.message.*;
import net.tinyos.packet.*;
import net.tinyos.util.*;

public class ReceiveSerial implements MessageListener {

  private MoteIF moteIF;

  public ReceiveSerial(MoteIF moteIF) {
    this.moteIF = moteIF;
    this.moteIF.registerListener(new SenseMsg(), this);
  }

  public void messageReceived(int to, Message message) {
    SenseMsg msg = (SenseMsg)message;
    int type = msg.get_kind();
    double tempature;
    double humidity;
    double photo;
    switch(type){
        case 0:
        	tempature = -40.1 + 0.01 * msg.get_data();
	        System.out.printf("received temperature:%.2f", tempature);
          System.out.println("℃C");
        	System.out.println();          
	        break;
        case 1:
        	humidity = -4 + 0.0405 * msg.get_data() + (-2.8/1000000) *msg.get_data() * msg.get_data();
          System.out.printf("received humidity:%.2f" , humidity);
          System.out.println("%");
        	System.out.println();           
	        break;
        case 2:
        	photo = msg.get_data() * 1.5 / 4096 / 10000;
        	photo = 0.625 * 1000000 * photo * 1000;
	        System.out.printf("received photo:%.2f" , photo);
        	System.out.println("Lux");
        	System.out.println();
	        break;
        default:
	        System.out.println("received unknow data:" + msg.get_data());
          break;
    }
    try {Thread.sleep(1000);}
    catch (InterruptedException exception) {}
  }

  private static void usage() {
    System.err.println("usage: ReceiveSerial [-comm <source>]");
  }

  public static void main(String[] args) throws Exception {
    String source = null;
    if (args.length == 2) {
      if (!args[0].equals("-comm")) {
	      usage();
	      System.exit(1);
      }
      source = args[1];
    }
    else if (args.length != 0) {
      usage();
      System.exit(1);
    }

    PhoenixSource phoenix;

    if (source == null) {
      phoenix = BuildSource.makePhoenix(PrintStreamMessenger.err);
    }
    else {
      phoenix = BuildSource.makePhoenix(source, PrintStreamMessenger.err);
    }

    MoteIF mif = new MoteIF(phoenix);
    ReceiveSerial serial = new ReceiveSerial(mif);    
  }
}
