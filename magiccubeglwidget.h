#ifndef MAGICCUBEGLWIDGET_H
#define MAGICCUBEGLWIDGET_H

#include <QGLWidget>
#include <GL/glu.h>

#define DRT_CLOCKWISE -1
#define DRT_COUNTERCLOCKWISE 1

#define FC_BLACK  0
#define FC_WHITE  -2
#define FC_RED    -4
#define FC_GREEN  -8
#define FC_YELLOW 2
#define FC_ORANGE 4
#define FC_BLUE   8

class SingleCube;
class easy_vector3;
class QTimer;
class Rotation;

class MagicCubeGlWidget : public QGLWidget
{
    Q_OBJECT
public:
    explicit MagicCubeGlWidget(QWidget *parent = 0);
    ~MagicCubeGlWidget();

signals:

public slots:
    void Flash_Rotate_Play();

protected:
    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();

    void keyPressEvent(QKeyEvent *event);
    void wheelEvent(QWheelEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

    void BuildCube(SingleCube *to_build);
    void Rotate_Face_Start(int face_color, int direction);

private:
    bool full_screen;
    bool key_operation_on;

    SingleCube *cube[3][3][3];//用来存储，指针指定对象后不发生变化

    SingleCube *cube_pointer_by_position[3][3][3];//用来指向某个位置的方格

    void pointer_change();
    /*
     * pointer_change()在动画结束的时候根据temp_face_color和temp_direction的值对cube_pointer_by_position
     * 的指针指向做出修改，转换一轮，同时对cube[][][]->center_position进行修改，使其符合新的位置，然后将
     * cube[][][]->rotate的信息清除
     */

    int pass_rotation_angle;
    int temp_face_color;
    int temp_direction;
    bool rotating;

    QTimer *flash_rotate_timer;

    Rotation *environment_rotate;//把绝对坐标系转换成环境坐标系的旋转变换
    GLfloat environment_zoom;

    QPoint temp_mouse_position;
    bool rightbutton_pressed;
    bool leftbutton_pressed;
};

#endif // MAGICCUBEGLWIDGET_H
