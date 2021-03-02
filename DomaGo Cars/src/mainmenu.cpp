#pragma comment(lib, "SHELL32.LIB")
#include <SFGUI/SFGUI.hpp>
#include <SFGUI/Widgets.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <windows.h>
#include <Shellapi.h>

#include "mainmenu.h"

void MainMenu::setAction(int action) {
	this->action = action;
}

int MainMenu::Run(sf::RenderWindow& App) {
	action = 0;
	
	// We have to do this because we don't use SFML to draw.
	App.resetGLStates();

	// Create an SFGUI. This is required before doing anything with SFGUI.
	sfg::SFGUI sfgui;

	// Create our main SFGUI window
	auto mainmenu_window = sfg::Window::Create(sfg::Window::Style::BACKGROUND);

	// Since only being able to add one widget to a window is very limiting
	// there are Box widgets. They are a subclass of the Container class and
	// can contain an unlimited amount of child widgets. Not only that, they
	// also have the ability to lay out your widgets nicely.

	// Create the box.
	// For layout purposes we must specify in what direction new widgets
	// should be added, horizontally or vertically.
	auto box = sfg::Box::Create(sfg::Box::Orientation::VERTICAL);
	auto logo_box = sfg::Window::Create(sfg::Window::Style::BACKGROUND);
	auto table = sfg::Table::Create();

	logo_box->SetRequisition(sf::Vector2f(1130.f, 350.f));
	box->SetRequisition(sf::Vector2f(1130.f, 625.f));

	sf::Image sfml_logo;

	// Our sfg::Image
	auto logo = sfg::Image::Create();

	// Try to load the image
	if (sfml_logo.loadFromFile("img/logo.png")) {
		logo->SetImage(sfml_logo);
		logo_box->Add(logo);
	}

	/*auto btn_CGP_l = sfg::Button::Create("Start CGP Learning");
	auto btn_CGP_s = sfg::Button::Create("Simulate CGP");
	auto btn_start = sfg::Button::Create("START");
	auto btn_settings = sfg::Button::Create("SETTINGS");
	auto btn_NN_l = sfg::Button::Create("Start NN Learning");
	auto btn_NN_s = sfg::Button::Create("Simulate NN");*/

	auto btn_ai_learning = sfg::Button::Create("AI LEARNING");
	auto btn_play = sfg::Button::Create("PLAY");
	auto btn_settings = sfg::Button::Create("SETTINGS");


	/*btn_start->GetSignal(sfg::Widget::OnLeftClick).Connect([this]() {
		setAction(1);
		});
	btn_NN_l->GetSignal(sfg::Widget::OnLeftClick).Connect([this]() {
		setAction(2);
		});
	btn_CGP_l->GetSignal(sfg::Widget::OnLeftClick).Connect([this]() {
		setAction(3);
		});
	btn_NN_s->GetSignal(sfg::Widget::OnLeftClick).Connect([this]() {
		setAction(4);
		});
	btn_CGP_s->GetSignal(sfg::Widget::OnLeftClick).Connect([this]() {
		setAction(5);
		});*/
	btn_ai_learning->GetSignal(sfg::Widget::OnLeftClick).Connect([this]() {
		setAction(7);
	});
	btn_play->GetSignal(sfg::Widget::OnLeftClick).Connect([this]() {
		setAction(6);
	});
	btn_settings->GetSignal(sfg::Widget::OnLeftClick).Connect([this]() {
		ShellExecute(
			NULL,
			"edit",
			"data\\dgc.cfg",
			NULL,
			NULL,
			SW_SHOWNORMAL
		);
		});

	// To add our widgets to the box we use the Pack() method instead of the
	// Add() method. This makes sure the widgets are added and layed out
	// properly in the box.
	

	// Attach a widget to the table.
	// The first parameter is the widget to attach.
	//
	// The second parameter tells the table how to place the widget.
	// It is a 4-tuple (ignore the fact that it is a rect) containing
	// in order: column index, row index, column span, row span.
	// ( 0, 0, 1, 1 ) would mean 0th column, 0th row, occupy 1 column and 1 row.
	//
	// Similar to boxes you have optional packing options such as FILL and EXPAND.
	// With tables you have 2 directions to pack, the first is the horizontal
	// packing options and the second the vertical packing options.
	//
	// The last option is the padding you want to apply to the cell.
	/*table->Attach(btn_CGP_l, sf::Rect<sf::Uint32>(0, 0, 1, 1), sfg::Table::FILL | sfg::Table::EXPAND, sfg::Table::FILL, sf::Vector2f(10.f, 50.f));
	table->Attach(btn_CGP_s, sf::Rect<sf::Uint32>(0, 1, 1, 1), sfg::Table::FILL | sfg::Table::EXPAND, sfg::Table::FILL, sf::Vector2f(10.f, 50.f));
	table->Attach(btn_start, sf::Rect<sf::Uint32>(1, 0, 1, 1), sfg::Table::FILL | sfg::Table::EXPAND, sfg::Table::FILL, sf::Vector2f(10.f, 50.f));
	table->Attach(btn_settings, sf::Rect<sf::Uint32>(1, 1, 1, 1), sfg::Table::FILL | sfg::Table::EXPAND, sfg::Table::FILL, sf::Vector2f(10.f, 50.f));
	table->Attach(btn_NN_l, sf::Rect<sf::Uint32>(2, 0, 1, 1), sfg::Table::FILL | sfg::Table::EXPAND, sfg::Table::FILL, sf::Vector2f(10.f, 50.f));
	table->Attach(btn_NN_s, sf::Rect<sf::Uint32>(2, 1, 1, 1), sfg::Table::FILL | sfg::Table::EXPAND, sfg::Table::FILL, sf::Vector2f(10.f, 50.f));*/

	table->Attach(btn_ai_learning, sf::Rect<sf::Uint32>(0, 0, 1, 1), sfg::Table::FILL | sfg::Table::EXPAND, sfg::Table::FILL, sf::Vector2f(10.f, 50.f));
	table->Attach(btn_play, sf::Rect<sf::Uint32>(1, 0, 1, 1), sfg::Table::FILL | sfg::Table::EXPAND, sfg::Table::FILL, sf::Vector2f(10.f, 50.f));
	table->Attach(btn_settings, sf::Rect<sf::Uint32>(2, 0, 1, 1), sfg::Table::FILL | sfg::Table::EXPAND, sfg::Table::FILL, sf::Vector2f(10.f, 50.f));

	table->SetRowSpacings(5.f);
	table->SetColumnSpacings(5.f);

	box->Pack(logo_box, true);
	box->PackEnd(table);

	// Finally we add our box to the window as it's only child.
	// Notice that we don't have to add the children of a box to it's parent
	// Because all children and grandchildren and .... are automatically
	// considered descendents of the parent.
	mainmenu_window->Add(box);


	// Start the game loop
	while (App.isOpen()) {
		// Process events
		sf::Event event;

		while (App.pollEvent(event)) {
			// Handle events
			mainmenu_window->HandleEvent(event);

			// Close window : exit
			if (event.type == sf::Event::Closed) {
				return -1;
			}
		}

		// Update the GUI, note that you shouldn't normally
		// pass 0 seconds to the update method.
		mainmenu_window->Update(0.f);

		// Clear screen
		App.clear();

		// Draw the GUI
		sfgui.Display(App);

		// Update the window
		App.display();

		switch (action) {
		case 6:
		case 7: return action;
		default: break;
		}
	}

	//Never reaching this point normally, but just in case, exit the application
	return -1;
}