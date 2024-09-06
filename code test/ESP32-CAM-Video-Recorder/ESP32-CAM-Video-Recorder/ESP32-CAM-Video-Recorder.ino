#include "record.h"


void the_camera_loop (void* pvParameter);
void the_sd_loop (void* pvParameter);

void setup() {

  Serial.begin(115200);
  Serial.println("\n\n---");


  Serial.println("Mounting the SD card ...");
  esp_err_t card_err = init_sdcard();
  if (card_err != ESP_OK) {
    Serial.printf("SD Card init failed with error 0x%x", card_err);
    major_fail();
    return;
  }


  Serial.println("Setting up the camera ...");
  config_camera();


  Serial.println("Checking SD for available space ...");
  delete_old_stuff();  


  Serial.println("Starting the wifi ...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");

  timeClient.begin();
  devstr.toCharArray(devname, devstr.length());         // name of your camera for mDNS, Router, and filenames
  framebuffer = (uint8_t*)ps_malloc(1024 * 1024);       // buffer to store a jpg in motion // needs to be larger for big frames from ov5640 

  Serial.println("Creating the_camera_loop_task");

  xTaskCreatePinnedToCore( the_camera_loop, "the_camera_loop", 3000, NULL, 3, &the_camera_loop_task, 0); // prio 3, core 0
  delay(100);

  Serial.println("  End of setup()\n\n");
}


void loop() {

}


void the_camera_loop (void* pvParameter) {

  frame_cnt = 0;
  start_record = 1;

  delay(500);

  while (1) {

    // if (frame_cnt == 0 && start_record == 0)  // do nothing
    // if (frame_cnt == 0 && start_record == 1)  // start a movie
    // if (frame_cnt > 0 && start_record == 0)   // stop the movie
    // if (frame_cnt > 0 && start_record != 0)   // another frame

    ///////////////////  NOTHING TO DO //////////////////
    if (frame_cnt == 0 && start_record == 0) {

      // Serial.println("Do nothing");
      delay(500);

      ///////////////////  START A MOVIE  //////////////////
    } else if (frame_cnt == 0 && start_record == 1) {

      Serial.println("Ready to start");

      delete_old_stuff();

      avi_start_time = millis();
      Serial.printf("\nStart the avi ... at %d\n", avi_start_time);
      Serial.printf("Framesize %d, quality %d, length %d seconds\n\n", framesize, quality, avi_length);

      frame_cnt++;

      fb_curr = get_good_jpeg();                     // should take zero time

      start_avi();

      fb_next = get_good_jpeg();                    // should take nearly zero time due to time spent writing header
      
      another_save_avi( fb_curr);                   // trigger sd write to write first frame


      ///////////////////  END THE MOVIE //////////////////
    } else if ( (frame_cnt > 0 && start_record == 0) ||  millis() > (avi_start_time + avi_length * 1000)) { // end the avi

      Serial.println("End the Avi");

      esp_camera_fb_return(fb_curr);

      frame_cnt++;
      fb_curr = fb_next;
      fb_next = NULL;

      another_save_avi( fb_curr);                   // save final frame of movie

      esp_camera_fb_return(fb_curr);
      fb_curr = NULL;

      end_avi();                                // end the movie

      avi_end_time = millis();

      float fps = 1.0 * frame_cnt / ((avi_end_time - avi_start_time) / 1000) ;

      Serial.printf("End the avi at %d.  It was %d frames, %d ms at %.2f fps...\n", millis(), frame_cnt, avi_end_time, avi_end_time - avi_start_time, fps);

      frame_cnt = 0;             // start recording again on the next loop

      ///////////////////  ANOTHER FRAME  //////////////////
    } else if (frame_cnt > 0 && start_record != 0) {  // another frame of the avi

      // Serial.println("Another frame");

      current_frame_time = millis();
      if (current_frame_time - last_frame_time < frame_interval) {
        delay(frame_interval - (current_frame_time - last_frame_time));             // delay for timelapse
      }
      last_frame_time = millis();

      frame_cnt++;

      esp_camera_fb_return(fb_curr);

      fb_curr = fb_next;           // we will write a frame, and get the camera preparing a new one

      another_save_avi( fb_curr);              // write the frame in fb_curr

      fb_next = get_good_jpeg();               // should take near zero, unless the sd is faster than the camera, when we will have to wait for the camera

      if (frame_cnt % 100 == 10 ) {     // print some status every 100 frames
        if (frame_cnt == 10) {
          bytes_before_last_100_frames = movi_size;
          time_before_last_100_frames = millis();
          most_recent_fps = 0;
          most_recent_avg_framesize = 0;
        } else {

          most_recent_fps = 100.0 / ((millis() - time_before_last_100_frames) / 1000.0) ;
          most_recent_avg_framesize = (movi_size - bytes_before_last_100_frames) / 100;

          if (Lots_of_Stats) {
            Serial.printf("So far: %04d frames, in %6.1f seconds, for last 100 frames: avg frame size %6.1f kb, %.2f fps ...\n", frame_cnt, 0.001 * (millis() - avi_start_time), 1.0 / 1024  * most_recent_avg_framesize, most_recent_fps);
          }

          total_delay = 0;

          bytes_before_last_100_frames = movi_size;
          time_before_last_100_frames = millis();
        }
      }
    }
  }
}

