#include "statemanager.h"

StateManager& StateManager::instance()
{
    static StateManager inst;
    return inst;
}

void StateManager::registerSceneProperty( const char* scene,
                                          QObject* obj,
                                          const char* propertyName,
                                          const QVariant& value )
{
    states[ scene ].append( { obj, propertyName, value } );
}

void StateManager::registerSceneCallback( const char* scene,
                                          std::function<void()> callback )
{
    callbacks[ scene ].append( callback );
}

void StateManager::switchScene( const char* scene )
{
    // 应用属性
    auto sceneStates = states.value( scene );
    for ( const auto& s : std::as_const( sceneStates ) )
    {
        QMetaObject::invokeMethod(
            s.obj,
            [ = ]() { s.obj->setProperty( s.propertyName, s.value ); },
            Qt::QueuedConnection );
    }

    // 执行回调
    auto sceneCallbacks = callbacks.value( scene );
    for ( const auto& cb : std::as_const( sceneCallbacks ) )
    {
        QMetaObject::invokeMethod(
            this, [ = ]() { cb(); }, Qt::QueuedConnection );
    }
}
