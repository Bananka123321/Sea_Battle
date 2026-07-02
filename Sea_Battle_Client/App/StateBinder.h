///Файл использовался для соблюдения SOLID
///Но как-будто лишний файл, пока пусть будет, можно сделать нормально удалив его

#pragma once

#include "AppState.h"
#include "Handler.h"

#include <QObject>

class StateChanger : public QObject {
    Q_OBJECT
public:
    StateChanger(Handler* handler, AppState* state);

private:
    Handler* handler_;
    AppState* state_;
};
