#include "modern_shell.h"

#include <QAction>
#include <QApplication>
#include <QCoreApplication>
#include <QEvent>
#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QPointer>
#include <QTimer>

namespace
{
	QPointer<modern_shell> g_preview_window;

	void show_modern_preview()
	{
		if (g_preview_window)
		{
			g_preview_window->show();
			g_preview_window->raise();
			g_preview_window->activateWindow();
			return;
		}

		auto* preview = new modern_shell();
		g_preview_window = preview;
		preview->setAttribute(Qt::WA_DeleteOnClose);
		preview->setWindowFlag(Qt::Window, true);
		preview->setWindowTitle(QObject::tr("RPCS3 Modern UI Preview"));
		preview->resize(1440, 860);
		preview->setMinimumSize(960, 640);
		preview->show();
	}

	QMenu* find_view_menu(QMainWindow* window)
	{
		if (!window)
		{
			return nullptr;
		}

		if (auto* menu = window->findChild<QMenu*>(QStringLiteral("menuView")))
		{
			return menu;
		}

		if (!window->menuBar())
		{
			return nullptr;
		}

		for (QAction* action : window->menuBar()->actions())
		{
			if (!action || !action->menu())
			{
				continue;
			}

			QString title = action->text();
			title.remove(QLatin1Char('&'));
			if (title.compare(QStringLiteral("View"), Qt::CaseInsensitive) == 0)
			{
				return action->menu();
			}
		}

		return nullptr;
	}

	bool attach_preview_action(QMainWindow* window)
	{
		if (!window)
		{
			return false;
		}

		QMenu* view_menu = find_view_menu(window);
		if (!view_menu)
		{
			return false;
		}

		if (!view_menu->findChild<QAction*>(QStringLiteral("modernUiPreviewAct")))
		{
			auto* preview_action = new QAction(QObject::tr("Modern UI Preview"), view_menu);
			preview_action->setObjectName(QStringLiteral("modernUiPreviewAct"));

			QAction* first_action = view_menu->actions().isEmpty() ? nullptr : view_menu->actions().constFirst();
			view_menu->insertAction(first_action, preview_action);
			view_menu->insertSeparator(first_action);

			QObject::connect(preview_action, &QAction::triggered, window, []
			{
				show_modern_preview();
			});
		}

		return true;
	}

	class modern_ui_event_filter final : public QObject
	{
	public:
		using QObject::QObject;

	protected:
		bool eventFilter(QObject* watched, QEvent* event) override
		{
			if (m_installed || !event || event->type() != QEvent::Show)
			{
				return QObject::eventFilter(watched, event);
			}

			auto* window = qobject_cast<QMainWindow*>(watched);
			if (!window || window->objectName() != QStringLiteral("main_window"))
			{
				return QObject::eventFilter(watched, event);
			}

			if (!attach_preview_action(window))
			{
				return QObject::eventFilter(watched, event);
			}

			m_installed = true;

			// Make preview builds unmistakable: open the modern shell automatically
			// the first time RPCS3's real main window is shown. The View menu action
			// remains available afterwards to reopen it.
			QTimer::singleShot(0, window, []
			{
				show_modern_preview();
			});

			return QObject::eventFilter(watched, event);
		}

	private:
		bool m_installed = false;
	};

	void install_modern_ui_bootstrap()
	{
		if (QCoreApplication* app = QCoreApplication::instance())
		{
			auto* filter = new modern_ui_event_filter(app);
			app->installEventFilter(filter);
		}
	}
}

Q_COREAPP_STARTUP_FUNCTION(install_modern_ui_bootstrap)
