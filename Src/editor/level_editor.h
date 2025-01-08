#ifndef __LEVEL_EDITOR_H__
#define __LEVEL_EDITOR_H__

#include "../gui/gui_system.h"
#include "../world.h"
#include <string>
#include <vector>

enum class EditorMode {
    SELECT,
    PLACE,
    PAINT,
    ERASE
};

enum class EditorTool {
    VOXEL,
    TERRAIN,
    LIGHT,
    ENTITY,
    PARTICLE_EMITTER
};

struct EditorCamera {
    Vector3 position;
    Vector3 rotation;
    float moveSpeed;
    float rotateSpeed;
    
    void Update(float deltaTime);
    void HandleInput();
};

class LevelEditor {
public:
    LevelEditor();
    ~LevelEditor();

    void Initialize();
    void Update(float deltaTime);
    void Render();
    void HandleInput(int x, int y, bool clicked);

    // Управление файлами
    void NewLevel();
    bool SaveLevel(const std::string& filename);
    bool LoadLevel(const std::string& filename);
    void ExportLevel(const std::string& filename);

    // Инструменты редактирования
    void SetMode(EditorMode mode);
    void SetTool(EditorTool tool);
    void Undo();
    void Redo();

private:
    // Состояние редактора
    EditorMode currentMode;
    EditorTool currentTool;
    bool isModified;
    std::string currentFileName;

    // Камера редактора
    EditorCamera camera;

    // Системы
    GUISystem gui;
    Window* mainWindow;
    Window* toolboxWindow;
    Window* propertiesWindow;

    // Выбранные объекты
    std::vector<void*> selectedObjects;

    // Настройки инструментов
    struct {
        Vector3 brushColor;
        float brushSize;
        float brushHardness;
        bool symmetryEnabled;
        Vector3 symmetryAxis;
    } toolSettings;

    // Интерфейс редактора
    void CreateMainWindow();
    void CreateToolbox();
    void CreatePropertiesPanel();
    void UpdateUI();

    // Операции редактирования
    void PlaceVoxel(const Vector3& position);
    void EraseVoxel(const Vector3& position);
    void PaintVoxel(const Vector3& position, const Vector3& color);
    void PlaceEntity(const Vector3& position);
    void PlaceLight(const Vector3& position);
    void PlaceParticleEmitter(const Vector3& position);

    // Работа с выделением
    void SelectObject(const Vector3& position);
    void DeselectAll();
    void DeleteSelected();
    void DuplicateSelected();

    // Вспомогательные функции
    Vector3 GetMouseWorldPosition(int x, int y);
    void UpdateGridVisibility();
    void UpdateSymmetryGuides();
    void ShowContextMenu(int x, int y);
};

#endif 