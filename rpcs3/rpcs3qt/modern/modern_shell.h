#pragma once

#include <QWidget>

class QLineEdit;
class QPushButton;
class QStackedWidget;

class modern_shell final : public QWidget
{
public:
	explicit modern_shell(QWidget* parent = nullptr);

	QStackedWidget* content_stack() const;

private:
	QWidget* create_sidebar();
	QWidget* create_top_bar();
	QWidget* create_placeholder_page(const QString& title, const QString& description);
	QPushButton* create_nav_button(const QString& text, int page_index);
	void apply_style();

	QStackedWidget* m_content_stack = nullptr;
	QLineEdit* m_search_bar = nullptr;
};
