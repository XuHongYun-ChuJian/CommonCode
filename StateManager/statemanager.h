// #pragma once
#include <QMap>
#include <QMetaObject>
#include <QObject>
#include <QString>
#include <QVariant>

#include <functional>

class StateManager : public QObject
{
    Q_OBJECT

public:
    static StateManager& instance();

    /**
     * @brief registerSceneProperty 注册某个场景下的属性值
     */
    void registerSceneProperty( const char* scene,
                                QObject* obj,
                                const char* propertyName,
                                const QVariant& value );

    /**
     * @brief registerSceneCallback 注册某个场景下的回调逻辑
     */
    void registerSceneCallback( const char* scene,
                                std::function<void()> callback );

    void switchScene( const char* scene );

private:
    struct StateItem
    {
        QObject* obj;
        const char* propertyName;
        QVariant value;
    };

    QMap<QString, QList<StateItem>> states;
    QMap<QString, QList<std::function<void()>>> callbacks;

    explicit StateManager( QObject* parent = nullptr )
        : QObject( parent )
    {}
    Q_DISABLE_COPY( StateManager )
};

#define STATE_MANAGER                                       StateManager::instance()
#define REGISTER_SCENE_PROPERTY( scene, obj, prop, value )  STATE_MANAGER.registerSceneProperty( scene, obj, prop, value )
#define REGISTER_SCENE_CALLBACK( scene, callback )          STATE_MANAGER.registerSceneCallback( scene, callback )
#define SWITCH_SCENE( scene )                               STATE_MANAGER.switchScene( scene )
