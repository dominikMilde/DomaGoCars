#include <SFGUI/SFGUI.hpp>
#include <SFGUI/Widgets.hpp>
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <Windows.h>

#include "NNLearning.h"
#include "main.h"
#include "neuralnetwork.h"
#include "storage.h"
#include "config.h"

void NNLearning::setAction(int action) {
	this->action = action;
}

void NNLearning::learn() {
	initConfig();
	init();
	vector<double> nn = runNN();
	storeNnDriver(nn);
	this->replace = true;
}

int NNLearning::Run(sf::RenderWindow& App) {
	action = 2;

	App.resetGLStates();
	sfg::SFGUI sfgui;

	sfg::Desktop desktop;

	auto l_window = sfg::Window::Create(sfg::Window::Style::BACKGROUND | sfg::Window::Style::CLOSE);
	auto l_box = sfg::Box::Create(sfg::Box::Orientation::VERTICAL);
	auto l_logo_box = sfg::Window::Create(sfg::Window::Style::BACKGROUND);

	l_logo_box->SetRequisition(sf::Vector2f(1130.f, 350.f));
	l_box->SetRequisition(sf::Vector2f(1130.f, 625.f));

	sf::Image l_sfml_logo;
	auto l_logo = sfg::Image::Create();
	if (l_sfml_logo.loadFromFile("img/logo.png")) {
		l_logo->SetImage(l_sfml_logo);
		l_logo_box->Add(l_logo);
	}

	auto l_label = sfg::Label::Create();
	l_label->SetText("Learning is a Lifelong Process...");
	l_box->Pack(l_logo_box, true);
	l_box->Pack(l_label);
	l_box->SetSpacing(20.f);
	l_window->Add(l_box);;

	auto f_window = sfg::Window::Create(sfg::Window::Style::BACKGROUND | sfg::Window::Style::CLOSE);
	auto f_box = sfg::Box::Create(sfg::Box::Orientation::VERTICAL);
	auto f_logo_box = sfg::Window::Create(sfg::Window::Style::BACKGROUND);

	f_logo_box->SetRequisition(sf::Vector2f(1130.f, 350.f));
	f_box->SetRequisition(sf::Vector2f(1130.f, 625.f));

	sf::Image f_sfml_logo;
	auto f_logo = sfg::Image::Create();
	if (f_sfml_logo.loadFromFile("img/logo.png")) {
		f_logo->SetImage(f_sfml_logo);
		f_logo_box->Add(f_logo);
	}

	auto f_table = sfg::Table::Create();

	auto f_label = sfg::Label::Create("Learning finished. Do you want to simulate trained driver?");
	auto btn_yes = sfg::Button::Create("YES");
	auto btn_no = sfg::Button::Create("NO");

	f_table->Attach(f_label, sf::Rect<sf::Uint32>(0, 0, 2, 1), sfg::Table::FILL | sfg::Table::EXPAND, sfg::Table::FILL, sf::Vector2f(10.f, 10.f));
	f_table->Attach(btn_yes, sf::Rect<sf::Uint32>(0, 1, 1, 1), sfg::Table::FILL | sfg::Table::EXPAND, sfg::Table::FILL, sf::Vector2f(10.f, 10.f));
	f_table->Attach(btn_no, sf::Rect<sf::Uint32>(1, 1, 1, 1), sfg::Table::FILL | sfg::Table::EXPAND, sfg::Table::FILL, sf::Vector2f(10.f, 10.f));
	f_table->SetRowSpacings(5.f);
	f_table->SetColumnSpacings(5.f);

	btn_yes->GetSignal(sfg::Widget::OnLeftClick).Connect([this]() {
		setAction(5);
		});
	btn_no->GetSignal(sfg::Widget::OnLeftClick).Connect([this]() {
		setAction(0);
		});

	f_box->Pack(f_logo_box, true);
	f_box->Pack(f_table);
	f_box->SetSpacing(50.f);
	f_window->Add(f_box);;

	desktop.Add(l_window);

	replace = false;

	sf::Thread Thread(&NNLearning::learn, this);
	Thread.launch();

	while (App.isOpen()) {
		sf::Event event;

		while (App.pollEvent(event)) {
			desktop.HandleEvent(event);

			// Close window : exit
			if (event.type == sf::Event::Closed) {
				return -1;
			}
		}

		desktop.Update(0.f);
		App.clear();
		sfgui.Display(App);
		App.display();

		if (replace) {
			desktop.Add(f_window);
			desktop.Remove(l_window);
			desktop.BringToFront(f_window);
			replace = false;
		}

		switch (action) {
		case 0:
		case 5: return action;
		default: break;
		}
	}

	return -1;
}