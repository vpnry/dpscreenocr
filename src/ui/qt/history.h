
#pragma once

#include <string>

#include <QTextBlockFormat>
#include <QTextCharFormat>
#include <QWidget>

#include "dpso_utils/dpso_utils.h"


class QCheckBox;
class QPushButton;
class QTextEdit;


class History : public QWidget {
    Q_OBJECT

public:
    explicit History(
        const std::string& cfgDirPath, QWidget* parent = nullptr);

    void append(const char* timestamp, const char* text);

    bool loadState();
    void saveState() const;
private slots:
    void setWordWrap(bool wordWrap);
    void clear();
    void saveAs();
private:
    struct DynamicStrings {
        QString clearQuestion;
        QString cancel;
        QString clear;

        QString saveHistory;
        QString nameFilters;

        DynamicStrings();
    } dynStr;

    std::string historyFilePath;
    dpso::HistoryUPtr history;

    QCheckBox* wordWrapCheck;

    QTextEdit* textEdit;
    QTextCharFormat charFormat;
    QTextBlockFormat blockFormat;
    int blockMargin;

    QPushButton* clearButton;
    QPushButton* saveAsButton;

    QString lastFileName;
    QString selectedNameFilter;

    void setButtonsEnabled(bool enabled);
    void appendToTextEdit(const char* text, const char* timestamp);
};
