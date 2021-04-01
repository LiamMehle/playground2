#include <gtk/gtk.h>
#include <stdio.h>


static void hello() {
	puts("hello!");
}

static gboolean delete_event( [[maybe_unused]] GtkWidget *widget,
                              [[maybe_unused]] GdkEvent  *event,
                              [[maybe_unused]] gpointer   data ) {
	g_print ("delete event occurred\n");
	gtk_main_quit();
	return false; // keep window?
}

int main( int argc, char** argv ) {

	gtk_init( &argc, &argv );

	GtkWidget* window = gtk_window_new( GTK_WINDOW_TOPLEVEL );
	GtkWidget* button = gtk_button_new_with_label("Hello, world!");

	g_signal_connect( window, "delete-event", G_CALLBACK(delete_event), NULL );
	g_signal_connect( button, "clicked"     , G_CALLBACK(hello), NULL );
	gtk_container_add( GTK_CONTAINER(window), button );
	gtk_widget_show( button );
	gtk_widget_show( window );
	gtk_main();
	return 0;
}