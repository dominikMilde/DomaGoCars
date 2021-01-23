#include <SFGUI/SFGUI.hpp>
#include <SFGUI/Widgets.hpp>

#include <SFML/Graphics.hpp>

int main() {
	// Create the main SFML window
	sf::RenderWindow app_window(sf::VideoMode(800, 600), "DomaGo Cars", sf::Style::Titlebar | sf::Style::Close);

	// We have to do this because we don't use SFML to draw.
	app_window.resetGLStates();

	// Create an SFGUI. This is required before doing anything with SFGUI.
	sfg::SFGUI sfgui;

	// Create our main SFGUI window
	auto window = sfg::Window::Create();

	// Since only being able to add one widget to a window is very limiting
	// there are Box widgets. They are a subclass of the Container class and
	// can contain an unlimited amount of child widgets. Not only that, they
	// also have the ability to lay out your widgets nicely.

	// Create the box.
	// For layout purposes we must specify in what direction new widgets
	// should be added, horizontally or vertically.
	auto box = sfg::Box::Create(sfg::Box::Orientation::VERTICAL);
	auto logo_box = sfg::Box::Create(sfg::Box::Orientation::HORIZONTAL);
	auto btns_box = sfg::Box::Create(sfg::Box::Orientation::VERTICAL);
	auto btns1_box = sfg::Box::Create(sfg::Box::Orientation::HORIZONTAL);
	auto btns2_box = sfg::Box::Create(sfg::Box::Orientation::HORIZONTAL);

	sf::Image sfml_logo;

	// Our sfg::Image
	auto logo = sfg::Image::Create();

	// Try to load the image
	if (sfml_logo.loadFromFile("logo.png")) {
		logo->SetImage(sfml_logo);
	}

	auto btn_CGP_l = sfg::Button::Create();
	auto btn_CGP_s = sfg::Button::Create();
	auto btn_start = sfg::Button::Create();
	auto btn_settings = sfg::Button::Create();
	auto btn_NN_l = sfg::Button::Create();
	auto btn_NN_s = sfg::Button::Create();

	btn_CGP_l->SetLabel("Start CGP Learning");
	btn_CGP_s->SetLabel("Simulate CGP");
	btn_start->SetLabel("START");
	btn_settings->SetLabel("SETTINGS");
	btn_NN_l->SetLabel("Start NN Learning");
	btn_NN_s->SetLabel("Simulate NN");

	// To add our widgets to the box we use the Pack() method instead of the
	// Add() method. This makes sure the widgets are added and layed out
	// properly in the box.
	logo_box->Pack(logo);

	btns1_box->Pack(btn_CGP_l);
	btns1_box->Pack(btn_start);
	btns1_box->Pack(btn_NN_l);
	btns2_box->Pack(btn_CGP_s);
	btns2_box->Pack(btn_settings);
	btns2_box->Pack(btn_NN_s);

	btns1_box->SetSpacing(20.f);
	btns2_box->SetSpacing(20.f);

	btns_box->Pack(btns1_box);
	btns_box->Pack(btns2_box);

	btns_box->SetSpacing(20.f);

	box->Pack(logo_box);
	box->Pack(btns_box);

	// Finally we add our box to the window as it's only child.
	// Notice that we don't have to add the children of a box to it's parent
	// Because all children and grandchildren and .... are automatically
	// considered descendents of the parent.
	window->Add(box);

	// Start the game loop
	while (app_window.isOpen()) {
		// Process events
		sf::Event event;

		while (app_window.pollEvent(event)) {
			// Handle events
			window->HandleEvent(event);

			// Close window : exit
			if (event.type == sf::Event::Closed) {
				return EXIT_SUCCESS;
			}
		}

		// Update the GUI, note that you shouldn't normally
		// pass 0 seconds to the update method.
		window->Update(0.f);

		// Clear screen
		app_window.clear();

		// Draw the GUI
		sfgui.Display(app_window);

		// Update the window
		app_window.display();
	}

	return EXIT_SUCCESS;
}