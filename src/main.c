#include <pebble.h>



Window *window;
TextLayer *text_layer;
InverterLayer *inv_layer;
char buffer[] = "00:00";

GBitmap  *backb_bitmap;
BitmapLayer *backb_layer;











void on_animation_stopped(Animation *anim, bool finished, void *context)
{
    //Free the memoery used by the Animation
    property_animation_destroy((PropertyAnimation*) anim);
}
 
void animate_layer(Layer *layer, GRect *start, GRect *finish, int duration, int delay)
{
    //Declare animation
    PropertyAnimation *anim = property_animation_create_layer_frame(layer, start, finish);
 
    //Set characteristics
    animation_set_duration((Animation*) anim, duration);
    animation_set_delay((Animation*) anim, delay);
 
    //Set stopped handler to free memory
    AnimationHandlers handlers = {
        //The reference to the stopped handler is the only one in the array
        .stopped = (AnimationStoppedHandler) on_animation_stopped
    };
    animation_set_handlers((Animation*) anim, handlers, NULL);
 
    //Start animation!
    animation_schedule((Animation*) anim);
}



void tick_handler(struct tm *tick_time, TimeUnits units_changed)
{
    //Format the buffer string using tick_time as the time source
    strftime(buffer, sizeof("00:00"), "%I:%M", tick_time);
 
    int seconds = tick_time->tm_sec;
 
    if(seconds == 29)
    {
        
        GRect startb = GRect(144, 110, 144, 168);
        GRect finishb = GRect(0, 110, 144, 168);
     
     ;
     animate_layer(bitmap_layer_get_layer(backb_layer), &startb, &finishb, 300, 500);
    
    }
    else if(seconds == 59)
    {
     
     GRect startb = GRect(0, 110, 144, 168);
     GRect finishb = GRect(144, 110, 144, 168);
     
     
     animate_layer(bitmap_layer_get_layer(backb_layer), &startb, &finishb, 300, 500);
    }
     else if(seconds == 0)
    {
        //Change the TextLayer text to show the new time!
        text_layer_set_text(text_layer, buffer);
     }
     else
    {
        //Change the TextLayer text to show the new time!
        text_layer_set_text(text_layer, buffer);
    }
     
     
     
   }
    
 
 



  
void window_load(Window *window)
{
 ResHandle font_handle = resource_get_handle(RESOURCE_ID_IMAGINE_42);
  //We will add the creation of the Window's elements here soon!
  text_layer = text_layer_create(GRect(5, 53, 144, 168));
  text_layer_set_background_color(text_layer, GColorClear);
  text_layer_set_text_color(text_layer, GColorBlack);
  text_layer_set_text_alignment(text_layer, GTextAlignmentCenter);
  text_layer_set_font(text_layer, fonts_load_custom_font(font_handle));
 
 
 
  //Load bitmaps into GBitmap structures

  backb_bitmap = gbitmap_create_with_resource(RESOURCE_ID_BACKB);
 
 
 
 //Create BitmapLayers to show GBitmaps and add to Window
 
 
 backb_layer = bitmap_layer_create(GRect(144, 113, 144, 168));
 bitmap_layer_set_bitmap(backb_layer, backb_bitmap);
 layer_add_child(window_get_root_layer(window), bitmap_layer_get_layer(backb_layer));
 
 
 
 layer_add_child(window_get_root_layer(window), (Layer*) text_layer);
  
 //Inverter layer
inv_layer = inverter_layer_create(GRect(0, 50, 144, 62));
layer_add_child(window_get_root_layer(window), (Layer*) inv_layer);
 
 
 
 
  

 //Get a time structure so that the face doesn't start blank
 struct tm *t;
 time_t temp;
 temp = time(NULL);
 t = localtime(&temp);
 
 //Manually call the tick handler when the window is loading
 tick_handler(t,  SECOND_UNIT);
}

  
void window_unload(Window *window)
{
  //We will safely destroy the Window's elements here!
    text_layer_destroy(text_layer);
 tick_timer_service_unsubscribe();
 inverter_layer_destroy(inv_layer);
 
}
  



 
 
 
 
 





void init()
{
  //Initialize the app elements here!
window = window_create();
  window_set_window_handlers(window, (WindowHandlers){
    .load = window_load,
      .unload = window_unload,
  });
window_stack_push(window, true);

 tick_timer_service_subscribe(MINUTE_UNIT, (TickHandler) tick_handler);
tick_timer_service_subscribe(SECOND_UNIT, (TickHandler) tick_handler);
 
}
  
void deinit()
{
  //De-initialize elements here to save memory!

  window_destroy(window);
}
  
  
  
  
int main(void)
{
  init();
  app_event_loop();
  deinit();
  
}