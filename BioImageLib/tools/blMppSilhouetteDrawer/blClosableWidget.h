#ifndef blClosableWidget_H
#define blClosableWidget_H

#include <QWidget>

class blClosableWidget : public QWidget
{
    Q_OBJECT
public:
    explicit blClosableWidget(QWidget *widget, QWidget *parent = 0);
    
    void setId(int id);
    void setIdString(QString id);
    int getId();
    QString getidString();


signals:
    void CloseMe();
    void CloseMe(int id);
    void CloseMe(int id, QString idStr);
    void CloseMe(QString isStr);
    
public slots:
    void EmitClose();
    QWidget* GetWidget();

private:
    int m_id;
    QString m_idString;
    QWidget *m_inputWidget;
    QWidget *closeBar;

protected:
    void enterEvent(QEvent *);
    void leaveEvent(QEvent *);
    
};

#endif // blClosableWidget_H
