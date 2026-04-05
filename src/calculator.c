#include <gtk/gtk.h>     // GTK header for GUI
#include <math.h>        // For math functions like sin, cos, sqrt
#include <string.h>      // For string operations
#include <stdlib.h>      // For atof(), malloc(), free()

// Global widgets
GtkWidget *entry;         // Entry widget for displaying input/output
GtkWidget *button_grid;   // Grid container for calculator buttons

// Function to evaluate basic math expressions
double evaluate_simple_expression(const char *expr, int *error) {
    double a, b;
    char op;
    *error = 0; // Reset error flag

    // Try parsing binary operations (e.g., 3 + 5)
    if (sscanf(expr, "%lf %c %lf", &a, &op, &b) == 3) {
        switch (op) {
            case '+': return a + b;
            case '-': return a - b;
            case '*': return a * b;
            case '/': 
                if (b == 0) {
                    *error = 1;  // Division by zero error
                    return 0;
                }
                return a / b;
            case '^': return pow(a, b);  // Power operation
            default: *error = 1; return 0;
        }
    } 
    // Handle scientific functions, in radians
    else if (strncmp(expr, "sin", 3) == 0) {
        return sin(atof(expr + 3));
    } else if (strncmp(expr, "cos", 3) == 0) {
        return cos(atof(expr + 3));
    } else if (strncmp(expr, "tan", 3) == 0) {
        return tan(atof(expr + 3));
    } else if (strncmp(expr, "log", 3) == 0) {
        double val = atof(expr + 3);
        if (val <= 0) {
            *error = 1;
            return 0;
        }
        return log10(val);  // Log base 10
    } else if (strncmp(expr, "sqrt", 4) == 0) {
        double val = atof(expr + 4);
        if (val < 0) {
            *error = 1;
            return 0;
        }
        return sqrt(val);
    }

    *error = 1;  // If input doesn't match any expected pattern
    return 0;
}

// Callback when a button is clicked
void on_button_clicked(GtkWidget *widget, gpointer data) {
    const gchar *label = gtk_button_get_label(GTK_BUTTON(widget));     // Get label of clicked button
    const gchar *current = gtk_editable_get_text(GTK_EDITABLE(entry)); // Current input in entry
    char buffer[256];

    if (strcmp(label, "=") == 0) {
        // Evaluate the expression
        int err = 0;
        double result = evaluate_simple_expression(current, &err);
        if (err) {
            gtk_editable_set_text(GTK_EDITABLE(entry), "Error");
        } else {
            snprintf(buffer, sizeof(buffer), "%.10g", result); // Format result
            gtk_editable_set_text(GTK_EDITABLE(entry), buffer);
        }
    } else if (strcmp(label, "C") == 0) {
        // Clear entry
        gtk_editable_set_text(GTK_EDITABLE(entry), "");
    } else if (strcmp(label, "DEL") == 0) {
        // Delete last character
        int len = strlen(current);
        if (len > 0) {
            char temp[256];
            strncpy(temp, current, len - 1);
            temp[len - 1] = '\0';
            gtk_editable_set_text(GTK_EDITABLE(entry), temp);
        }
    } else {
        // Append button label to current entry text
        snprintf(buffer, sizeof(buffer), "%s%s", current, label);
        gtk_editable_set_text(GTK_EDITABLE(entry), buffer);
    }
}

// Function to create individual buttons
GtkWidget *create_button(const char *label) {
    GtkWidget *btn = gtk_button_new_with_label(label); // Creat button
    gtk_widget_set_size_request(btn, 80, 60);  // Set fixed button size
    GtkStyleContext *context = gtk_widget_get_style_context(btn);
    gtk_style_context_add_class(context, "calc-button");  // Add custom style class
    g_signal_connect(btn, "clicked", G_CALLBACK(on_button_clicked), NULL); // Connect button click
    return btn;
}

// Function to set up and display the GUI
void activate(GtkApplication *app, gpointer user_data) {
    // Create main window
    GtkWidget *window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Calculator");
    gtk_window_set_default_size(GTK_WINDOW(window), 400, 600);
    gtk_window_set_resizable(GTK_WINDOW(window), FALSE);

    // Vertical box to hold all UI elements
    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_widget_set_margin_top(vbox, 20);
    gtk_widget_set_margin_bottom(vbox, 20);
    gtk_widget_set_margin_start(vbox, 20);
    gtk_widget_set_margin_end(vbox, 20);

    // Entry field for displaying calculations
    entry = gtk_entry_new();
    gtk_widget_set_size_request(entry, -1, 50);
    gtk_editable_set_editable(GTK_EDITABLE(entry), FALSE); // Read-only entry
    gtk_box_append(GTK_BOX(vbox), entry);

    // Container for button grid
    GtkWidget *grid_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);

    // Define button labels in rows
    const char *buttons[6][4] = {
        {"7", "8", "9", "*"},
        {"4", "5", "6", "-"},
        {"1", "2", "3", "+"},
        {"/", "0", ".", "="},
        {"sin", "cos", "tan", "log"},
        {"^", "sqrt ", "DEL", "C"}
    };

    // Create button layout
    for (int i = 0; i < 6; ++i) {
        GtkWidget *hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10); // Row container
        for (int j = 0; j < 4; ++j) {
            gtk_box_append(GTK_BOX(hbox), create_button(buttons[i][j]));
        }
        gtk_box_append(GTK_BOX(grid_box), hbox); // Add row to main button grid
    }

    // Optional animation using GtkRevealer
    GtkWidget *revealer = gtk_revealer_new();
    gtk_revealer_set_transition_type(GTK_REVEALER(revealer), GTK_REVEALER_TRANSITION_TYPE_SLIDE_DOWN);
    gtk_revealer_set_transition_duration(GTK_REVEALER(revealer), 1000); // Animation duration (1 sec)
    gtk_revealer_set_child(GTK_REVEALER(revealer), grid_box); // Put grid inside the revealer
    gtk_box_append(GTK_BOX(vbox), revealer); // Add to main vertical box

    // Finalize window layout
    gtk_window_set_child(GTK_WINDOW(window), vbox);
    gtk_window_present(GTK_WINDOW(window));

    // Start animation after delay
    g_timeout_add(200, (GSourceFunc)gtk_revealer_set_reveal_child, revealer);
    gtk_revealer_set_reveal_child(GTK_REVEALER(revealer), TRUE);

    // Optional: Enable dark theme
    GtkSettings *settings = gtk_settings_get_default();
    g_object_set(settings, "gtk-application-prefer-dark-theme", TRUE, NULL);
}

// Main function: entry point
int main(int argc, char *argv[]) {
    GtkApplication *app = gtk_application_new("com.shruti.animatedcalc", G_APPLICATION_DEFAULT_FLAGS); // Create app
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL); // Connect activate signal
    int status = g_application_run(G_APPLICATION(app), argc, argv); // Run the app
    g_object_unref(app); // Free memory
    return status; // Return status code
}

//Shruti@LAPTOP-G6T83077 UCRT64 ~
//$ cd "/c/Users/Shruti/OneDrive/one drive/c codes"
//
//Shruti@LAPTOP-G6T83077 UCRT64 /c/Users/Shruti/OneDrive/one drive/c codes
//$ gcc -o cal cal.c `pkg-config --cflags --libs gtk4`
//
//Shruti@LAPTOP-G6T83077 UCRT64 /c/Users/Shruti/OneDrive/one drive/c codes
//$ ./cal
