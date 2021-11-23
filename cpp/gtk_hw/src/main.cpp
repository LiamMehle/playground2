#include <stdio.h>
#include <gtk/gtk.h>
#include <thread>


static int constexpr default_size_x = 600;
static int constexpr default_size_y = 400;

static
void button_print(GtkButton const * button, gpointer msg) {
	puts((char const *) msg);
}

// basically main for gtk apps
static
void on_app_activate(GApplication *app, gpointer data) {
	auto* window = gtk_application_window_new(GTK_APPLICATION(app));
	auto* button = gtk_button_new_with_label("print \"Hello, world!\"");
	g_signal_connect(button, "clicked", G_CALLBACK(button_print), (char*)"Hello, world!");
	// button setup is complete, now add to window
	gtk_container_add(GTK_CONTAINER(window), button);
	gtk_widget_show_all(window);
}

int main(int argc, char** argv) {
	char const window_name[] = "hello.world";
	GtkApplication *app = gtk_application_new(window_name, G_APPLICATION_FLAGS_NONE);
	g_signal_connect(app, "activate", G_CALLBACK(on_app_activate), nullptr);

	int const status = g_application_run(G_APPLICATION(app), argc, argv);
	g_object_unref(app);
	return status;
}
