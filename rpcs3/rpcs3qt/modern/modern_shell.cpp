#include "modern_shell.h"

#include <QFrame>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QStackedWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>

modern_shell::modern_shell(QWidget* parent)
	: QWidget(parent)
{
	setObjectName(QStringLiteral("modernShell"));

	auto* root_layout = new QHBoxLayout(this);
	root_layout->setContentsMargins(0, 0, 0, 0);
	root_layout->setSpacing(0);
	root_layout->addWidget(create_sidebar());

	auto* main_panel = new QWidget(this);
	main_panel->setObjectName(QStringLiteral("modernMainPanel"));

	auto* main_layout = new QVBoxLayout(main_panel);
	main_layout->setContentsMargins(24, 18, 24, 24);
	main_layout->setSpacing(18);
	main_layout->addWidget(create_top_bar());

	m_content_stack = new QStackedWidget(main_panel);
	m_content_stack->setObjectName(QStringLiteral("modernContentStack"));

	m_content_stack->addWidget(create_placeholder_page(
		tr("Home"),
		tr("A clean dashboard for recently played games, updates, RPCN status and quick actions.")));
	m_content_stack->addWidget(create_placeholder_page(
		tr("Library"),
		tr("RPCS3's real game data will power a modern cover-based library here.")));
	m_content_stack->addWidget(create_placeholder_page(
		tr("Recent"),
		tr("Recently played games and resumable sessions will live here.")));
	m_content_stack->addWidget(create_placeholder_page(
		tr("Favorites"),
		tr("Pinned games and personal collections will be surfaced here.")));
	m_content_stack->addWidget(create_placeholder_page(
		tr("Patches"),
		tr("Game patches and per-title configuration will be easier to discover and manage.")));
	m_content_stack->addWidget(create_placeholder_page(
		tr("Controllers"),
		tr("Controller configuration will be rebuilt around a visual, device-first workflow.")));
	m_content_stack->addWidget(create_placeholder_page(
		tr("RPCN"),
		tr("RPCS3's existing RPCN implementation will remain intact behind a cleaner online interface.")));
	m_content_stack->addWidget(create_placeholder_page(
		tr("Settings"),
		tr("Global and per-game settings will be reorganized without changing the emulator core.")));
	m_content_stack->addWidget(create_placeholder_page(
		tr("Tools"),
		tr("Advanced RPCS3 utilities and debugging tools remain available without crowding the main library.")));

	main_layout->addWidget(m_content_stack, 1);
	root_layout->addWidget(main_panel, 1);

	apply_style();
}

QStackedWidget* modern_shell::content_stack() const
{
	return m_content_stack;
}

QWidget* modern_shell::create_sidebar()
{
	auto* sidebar = new QFrame(this);
	sidebar->setObjectName(QStringLiteral("modernSidebar"));
	sidebar->setFixedWidth(220);

	auto* layout = new QVBoxLayout(sidebar);
	layout->setContentsMargins(18, 22, 18, 18);
	layout->setSpacing(8);

	auto* brand = new QLabel(QStringLiteral("RPCS3"), sidebar);
	brand->setObjectName(QStringLiteral("modernBrand"));
	layout->addWidget(brand);

	auto* subtitle = new QLabel(tr("Modern UI Preview"), sidebar);
	subtitle->setObjectName(QStringLiteral("modernBrandSubtitle"));
	layout->addWidget(subtitle);
	layout->addSpacing(20);

	const QStringList nav_items =
	{
		tr("Home"),
		tr("Library"),
		tr("Recent"),
		tr("Favorites"),
		tr("Patches"),
		tr("Controllers"),
		tr("RPCN"),
		tr("Settings"),
		tr("Tools")
	};

	for (int index = 0; index < nav_items.size(); ++index)
	{
		layout->addWidget(create_nav_button(nav_items.at(index), index));
	}

	layout->addStretch();

	auto* status = new QLabel(tr("Experimental frontend\nEmulation core unchanged"), sidebar);
	status->setObjectName(QStringLiteral("modernSidebarStatus"));
	status->setWordWrap(true);
	layout->addWidget(status);

	return sidebar;
}

QWidget* modern_shell::create_top_bar()
{
	auto* top_bar = new QWidget(this);
	top_bar->setObjectName(QStringLiteral("modernTopBar"));

	auto* layout = new QHBoxLayout(top_bar);
	layout->setContentsMargins(0, 0, 0, 0);
	layout->setSpacing(12);

	m_search_bar = new QLineEdit(top_bar);
	m_search_bar->setObjectName(QStringLiteral("modernSearchBar"));
	m_search_bar->setPlaceholderText(tr("Search games, serials or settings..."));
	m_search_bar->setClearButtonEnabled(true);
	layout->addWidget(m_search_bar, 1);

	auto* add_game = new QPushButton(tr("Add Game"), top_bar);
	add_game->setObjectName(QStringLiteral("modernPrimaryButton"));
	layout->addWidget(add_game);

	return top_bar;
}

QWidget* modern_shell::create_placeholder_page(const QString& title, const QString& description)
{
	auto* page = new QFrame(this);
	page->setObjectName(QStringLiteral("modernPage"));

	auto* layout = new QVBoxLayout(page);
	layout->setContentsMargins(32, 30, 32, 30);
	layout->setSpacing(10);

	auto* heading = new QLabel(title, page);
	heading->setObjectName(QStringLiteral("modernPageTitle"));
	layout->addWidget(heading);

	auto* body = new QLabel(description, page);
	body->setObjectName(QStringLiteral("modernPageDescription"));
	body->setWordWrap(true);
	body->setMaximumWidth(760);
	layout->addWidget(body);

	layout->addStretch();
	return page;
}

QPushButton* modern_shell::create_nav_button(const QString& text, int page_index)
{
	auto* button = new QPushButton(text, this);
	button->setObjectName(QStringLiteral("modernNavButton"));
	button->setCheckable(true);
	button->setAutoExclusive(true);
	button->setCursor(Qt::PointingHandCursor);

	if (page_index == 0)
	{
		button->setChecked(true);
	}

	connect(button, &QPushButton::clicked, this, [this, page_index]
	{
		if (m_content_stack)
		{
			m_content_stack->setCurrentIndex(page_index);
		}
	});

	return button;
}

void modern_shell::apply_style()
{
	setStyleSheet(QStringLiteral(R"(
		#modernShell {
			background: #07101f;
			color: #f4f7ff;
		}
		#modernSidebar {
			background: #09152a;
			border-right: 1px solid #1d2b48;
		}
		#modernBrand {
			font-size: 25px;
			font-weight: 700;
			letter-spacing: 1px;
			color: #f7f8ff;
		}
		#modernBrandSubtitle {
			font-size: 11px;
			color: #8292b4;
		}
		#modernNavButton {
			min-height: 42px;
			padding: 0 14px;
			border: 1px solid transparent;
			border-radius: 10px;
			background: transparent;
			color: #aebbd5;
			font-size: 13px;
			font-weight: 500;
			text-align: left;
		}
		#modernNavButton:hover {
			background: #101f3b;
			color: #ffffff;
		}
		#modernNavButton:checked {
			background: #263b8f;
			border-color: #546dff;
			color: #ffffff;
		}
		#modernSidebarStatus {
			padding: 12px;
			border: 1px solid #20304f;
			border-radius: 10px;
			background: #0d1a31;
			color: #7f90b3;
			font-size: 10px;
		}
		#modernMainPanel {
			background: #07101f;
		}
		#modernSearchBar {
			min-height: 42px;
			padding: 0 14px;
			border: 1px solid #233456;
			border-radius: 11px;
			background: #0d1a31;
			color: #f5f7ff;
			selection-background-color: #5865f2;
		}
		#modernSearchBar:focus {
			border-color: #6578ff;
		}
		#modernPrimaryButton {
			min-height: 42px;
			padding: 0 18px;
			border: 1px solid #6f61ff;
			border-radius: 11px;
			background: #5b4cf4;
			color: white;
			font-weight: 600;
		}
		#modernPrimaryButton:hover {
			background: #6b5cff;
		}
		#modernPage {
			border: 1px solid #1d2d4b;
			border-radius: 16px;
			background: #0a162a;
		}
		#modernPageTitle {
			font-size: 28px;
			font-weight: 700;
			color: #f7f9ff;
		}
		#modernPageDescription {
			font-size: 13px;
			color: #94a4c5;
		}
	)"));
}
