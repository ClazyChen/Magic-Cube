#include "magiccubeglwidget.h"
#include "singlecube.h"
#include "rotation.h"
#include <QKeyEvent>
#include <cmath>
#include <QWheelEvent>
#include <QTimer>
#include <QDebug>

#define HALF_EDGE_LENGTH 0.975f
#define HALF_COLOR_LENGTH 0.9f
#define HALF_FLOAT_LENGTH 0.976f

#define ROTATE_STEP_LENGTH 3.0f

//#define TEST_ROTATE

MagicCubeGlWidget::MagicCubeGlWidget(QWidget *parent) :
    QGLWidget(parent)
{
    full_screen = false;
    key_operation_on = true;
    rotating = false;
    rightbutton_pressed = false;
    leftbutton_pressed = false;
    pass_rotation_angle = 0.0f;
    environment_rotate = new Rotation();
    environment_zoom = 12.0f;

    //initialize cubes
    int i, j, k;
    for (i = 0; i < 3; ++ i)
        for (j = 0; j < 3; ++ j)
            for (k = 0; k < 3; ++ k)
            {
                cube[i][j][k] = new SingleCube();
                cube[i][j][k] ->center_position =
                        easy_vector3((GLfloat)i*2.0f-2.0f,
                                     (GLfloat)j*2.0f-2.0f,
                                     (GLfloat)k*2.0f-2.0f);
                cube[i][j][k] ->rotate = Rotation();
                cube[i][j][k] ->colors = cube_color();
                cube_pointer_by_position[i][j][k] = cube[i][j][k];
            }
    for (i = 0; i < 3; ++ i)
        for (j = 0; j < 3; ++ j)
        {
            cube[i][j][2] ->colors.face = MCC_YELLOW;
            cube[i][j][0] ->colors.back = MCC_WHITE;
            cube[i][0][j] ->colors.left = MCC_ORANGE;
            cube[i][2][j] ->colors.right = MCC_RED;
            cube[0][i][j] ->colors.top = MCC_GREEN;
            cube[2][i][j] ->colors.bottom = MCC_BLUE;
        }
}

MagicCubeGlWidget::~MagicCubeGlWidget()
{
}

void MagicCubeGlWidget::initializeGL()
{
    glClearColor(0.8, 0.8, 0.8, 0.0);
    glShadeModel(GL_SMOOTH);

    glClearDepth(1.0);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
}

void MagicCubeGlWidget::resizeGL(int w, int h)
{
    glViewport(0, 0, (GLint)w, (GLint)h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (GLfloat)w/(GLfloat)h, 0.1, 100.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void MagicCubeGlWidget::Flash_Rotate_Play()
{
    if (rotating)
    {
        int i, j;
        switch (temp_face_color)
        {
        case FC_YELLOW:
            for (i = 0; i < 3; ++ i)
                for (j = 0; j < 3; ++ j)
                    cube_pointer_by_position[i][j][2] ->compos_rotation(cube[1][1][2] ->center_position, temp_direction*ROTATE_STEP_LENGTH);
            break;
        case FC_WHITE:
            for (i = 0; i < 3; ++ i)
                for (j = 0; j < 3; ++ j)
                    cube_pointer_by_position[i][j][0] ->compos_rotation(cube[1][1][0] ->center_position, temp_direction*ROTATE_STEP_LENGTH);
            break;
        case FC_BLUE:
            for (i = 0; i < 3; ++ i)
                for (j = 0; j < 3; ++ j)
                    cube_pointer_by_position[2][i][j] ->compos_rotation(cube[2][1][1] ->center_position, temp_direction*ROTATE_STEP_LENGTH);
            break;
        case FC_GREEN:
            for (i = 0; i < 3; ++ i)
                for (j = 0; j < 3; ++ j)
                    cube_pointer_by_position[0][i][j] ->compos_rotation(cube[0][1][1] ->center_position, temp_direction*ROTATE_STEP_LENGTH);
            break;
        case FC_RED:
            for (i = 0; i < 3; ++ i)
                for (j = 0; j < 3; ++ j)
                    cube_pointer_by_position[i][2][j] ->compos_rotation(cube[1][2][1] ->center_position, temp_direction*ROTATE_STEP_LENGTH);
            break;
        case FC_ORANGE:
            for (i = 0; i < 3; ++ i)
                for (j = 0; j < 3; ++ j)
                    cube_pointer_by_position[i][0][j] ->compos_rotation(cube[1][0][1] ->center_position, temp_direction*ROTATE_STEP_LENGTH);
            break;
        default:
            return;
        }
        updateGL();
        pass_rotation_angle += (int)ROTATE_STEP_LENGTH;
        if (pass_rotation_angle >= 90)
        {
            rotating = false;
            flash_rotate_timer ->stop();
            delete flash_rotate_timer;
            pointer_change();
            updateGL();
            key_operation_on = true;
        }
    }
}

void MagicCubeGlWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    int i, j, k;
    for (i = 0; i < 3; ++ i)
        for (j = 0; j < 3; ++ j)
            for (k = 0; k < 3; ++ k)
                BuildCube(cube[i][j][k]);
}

void MagicCubeGlWidget::keyPressEvent(QKeyEvent *event)
{
    if (!key_operation_on)
        return;
    switch (event ->key())
    {
    case Qt::Key_F1:
        full_screen = !full_screen;
        if (full_screen)
        {
            showFullScreen();
        }
        else
        {
            showNormal();
        }
        updateGL();
        break;
    case Qt::Key_Escape:
        close();
        break;
    case 'Y':
        Rotate_Face_Start(FC_YELLOW, DRT_CLOCKWISE);
        break;
    case 'U':
        Rotate_Face_Start(FC_YELLOW, DRT_COUNTERCLOCKWISE);
        break;
    case 'T':
        Rotate_Face_Start(FC_WHITE, DRT_COUNTERCLOCKWISE);
        break;
    case 'R':
        Rotate_Face_Start(FC_WHITE, DRT_CLOCKWISE);
        break;
    case 'H':
        Rotate_Face_Start(FC_BLUE, DRT_CLOCKWISE);
        break;
    case 'J':
        Rotate_Face_Start(FC_BLUE, DRT_COUNTERCLOCKWISE);
        break;
    case 'F':
        Rotate_Face_Start(FC_GREEN, DRT_CLOCKWISE);
        break;
    case 'G':
        Rotate_Face_Start(FC_GREEN, DRT_COUNTERCLOCKWISE);
        break;
    case 'N':
        Rotate_Face_Start(FC_RED, DRT_CLOCKWISE);
        break;
    case 'M':
        Rotate_Face_Start(FC_RED, DRT_COUNTERCLOCKWISE);
        break;
    case 'V':
        Rotate_Face_Start(FC_ORANGE, DRT_CLOCKWISE);
        break;
    case 'B':
        Rotate_Face_Start(FC_ORANGE, DRT_COUNTERCLOCKWISE);
        break;
    }
}

void MagicCubeGlWidget::wheelEvent(QWheelEvent *event)
{
    if (event ->delta() > 0)
    {
        environment_zoom -= 1.0f;
        if (environment_zoom < 5.0f)
            environment_zoom = 5.0f;
    }
    else
    {
        environment_zoom += 1.0f;
        if (environment_zoom > 100.0f)
            environment_zoom = 100.0f;
    }
    updateGL();
}

void MagicCubeGlWidget::mousePressEvent(QMouseEvent *event)
{
    if (event ->button() & Qt::RightButton)
    {
        rightbutton_pressed = true;
        temp_mouse_position = event ->pos();
    }
    if (event ->button() & Qt::LeftButton)
    {
        leftbutton_pressed = true;
        temp_mouse_position = event ->pos();
    }
}

void MagicCubeGlWidget::mouseMoveEvent(QMouseEvent *event)
{
    if (rightbutton_pressed)
    {
        QPoint new_mouse_position = event ->pos() - temp_mouse_position;
        (*environment_rotate) =
                 Rotation(easy_vector3(1.0f, 0.0f, 0.0f), (GLfloat)new_mouse_position.y()/5.0f)
                * Rotation(easy_vector3(0.0f, 1.0f, 0.0f), (GLfloat)new_mouse_position.x()/5.0f)
                * (*environment_rotate);
        temp_mouse_position = event ->pos();
        updateGL();
    }
    if (leftbutton_pressed)
    {
        QPoint now_mouse_position = event ->pos();
        double center_x = (double)width()/2.0;
        double center_y = (double)height()/2.0;
        double now_x = (double)now_mouse_position.x() - center_x;
        double now_y = (double)now_mouse_position.y() - center_y;
        double temp_x = (double)temp_mouse_position.x() - center_x;
        double temp_y = (double)temp_mouse_position.y() - center_y;
        double temp_angle = temp_x * temp_x + temp_y * temp_y == 0.0 ? 0.0 :
                            acos(temp_x / sqrt(temp_x * temp_x + temp_y * temp_y));
        double now_angle = now_x * now_x + now_y * now_y == 0.0 ? 0.0 :
                            acos(now_x / sqrt(now_x * now_x + now_y * now_y));
        GLfloat rotate_angle = 180.0f / (GLfloat)M_PI * (now_angle - temp_angle);
        (*environment_rotate) =
                Rotation(easy_vector3(0.0f, 0.0f, (now_y < 0 ? 1 : -1) * 1.0f), rotate_angle)
                * (*environment_rotate);
        temp_mouse_position = event ->pos();
        updateGL();
    }
}

void MagicCubeGlWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if (event ->button() & Qt::RightButton)
    {
        rightbutton_pressed = false;
    }
    if (event ->button() & Qt::LeftButton)
    {
        leftbutton_pressed = false;
    }
}

void MagicCubeGlWidget::BuildCube(SingleCube *to_build)
{
    glLoadIdentity();

    glTranslatef(0.0f, 0.0f, -environment_zoom);
    glRotatef(environment_rotate ->angle, environment_rotate ->axis.x,
                                          environment_rotate ->axis.y,
                                          environment_rotate ->axis.z);
    glRotatef(to_build ->rotate.angle, to_build ->rotate.axis.x,
                                       to_build ->rotate.axis.y,
                                       to_build ->rotate.axis.z);
    glTranslatef(to_build ->center_position.x,
                 to_build ->center_position.y,
                 to_build ->center_position.z);

    glBegin(GL_QUADS);

        glColor3f((MCC_BLACK).x, (MCC_BLACK).y, (MCC_BLACK).z);
        glVertex3f(HALF_EDGE_LENGTH, HALF_EDGE_LENGTH, HALF_EDGE_LENGTH);
        glVertex3f(-HALF_EDGE_LENGTH, HALF_EDGE_LENGTH, HALF_EDGE_LENGTH);
        glVertex3f(-HALF_EDGE_LENGTH, -HALF_EDGE_LENGTH, HALF_EDGE_LENGTH);
        glVertex3f(HALF_EDGE_LENGTH, -HALF_EDGE_LENGTH, HALF_EDGE_LENGTH);

        glVertex3f(HALF_EDGE_LENGTH, HALF_EDGE_LENGTH, -HALF_EDGE_LENGTH);
        glVertex3f(HALF_EDGE_LENGTH, -HALF_EDGE_LENGTH, -HALF_EDGE_LENGTH);
        glVertex3f(-HALF_EDGE_LENGTH, -HALF_EDGE_LENGTH, -HALF_EDGE_LENGTH);
        glVertex3f(-HALF_EDGE_LENGTH, HALF_EDGE_LENGTH, -HALF_EDGE_LENGTH);

        glVertex3f(HALF_EDGE_LENGTH, -HALF_EDGE_LENGTH, HALF_EDGE_LENGTH);
        glVertex3f(-HALF_EDGE_LENGTH, -HALF_EDGE_LENGTH, HALF_EDGE_LENGTH);
        glVertex3f(-HALF_EDGE_LENGTH, -HALF_EDGE_LENGTH, -HALF_EDGE_LENGTH);
        glVertex3f(HALF_EDGE_LENGTH, -HALF_EDGE_LENGTH, -HALF_EDGE_LENGTH);

        glVertex3f(HALF_EDGE_LENGTH, HALF_EDGE_LENGTH, HALF_EDGE_LENGTH);
        glVertex3f(HALF_EDGE_LENGTH, HALF_EDGE_LENGTH, -HALF_EDGE_LENGTH);
        glVertex3f(-HALF_EDGE_LENGTH, HALF_EDGE_LENGTH, -HALF_EDGE_LENGTH);
        glVertex3f(-HALF_EDGE_LENGTH, HALF_EDGE_LENGTH, HALF_EDGE_LENGTH);

        glVertex3f(-HALF_EDGE_LENGTH, HALF_EDGE_LENGTH, HALF_EDGE_LENGTH);
        glVertex3f(-HALF_EDGE_LENGTH, HALF_EDGE_LENGTH, -HALF_EDGE_LENGTH);
        glVertex3f(-HALF_EDGE_LENGTH, -HALF_EDGE_LENGTH, -HALF_EDGE_LENGTH);
        glVertex3f(-HALF_EDGE_LENGTH, -HALF_EDGE_LENGTH, HALF_EDGE_LENGTH);

        glVertex3f(HALF_EDGE_LENGTH, HALF_EDGE_LENGTH, HALF_EDGE_LENGTH);
        glVertex3f(HALF_EDGE_LENGTH, -HALF_EDGE_LENGTH, HALF_EDGE_LENGTH);
        glVertex3f(HALF_EDGE_LENGTH, -HALF_EDGE_LENGTH, -HALF_EDGE_LENGTH);
        glVertex3f(HALF_EDGE_LENGTH, HALF_EDGE_LENGTH, -HALF_EDGE_LENGTH);

        glColor3f(to_build ->colors.face.x,
                  to_build ->colors.face.y,
                  to_build ->colors.face.z);
        glVertex3f(HALF_COLOR_LENGTH, HALF_COLOR_LENGTH, HALF_FLOAT_LENGTH);
        glVertex3f(-HALF_COLOR_LENGTH, HALF_COLOR_LENGTH, HALF_FLOAT_LENGTH);
        glVertex3f(-HALF_COLOR_LENGTH, -HALF_COLOR_LENGTH, HALF_FLOAT_LENGTH);
        glVertex3f(HALF_COLOR_LENGTH, -HALF_COLOR_LENGTH, HALF_FLOAT_LENGTH);

        glColor3f(to_build ->colors.back.x,
                  to_build ->colors.back.y,
                  to_build ->colors.back.z);
        glVertex3f(HALF_COLOR_LENGTH, HALF_COLOR_LENGTH, -HALF_FLOAT_LENGTH);
        glVertex3f(-HALF_COLOR_LENGTH, HALF_COLOR_LENGTH, -HALF_FLOAT_LENGTH);
        glVertex3f(-HALF_COLOR_LENGTH, -HALF_COLOR_LENGTH, -HALF_FLOAT_LENGTH);
        glVertex3f(HALF_COLOR_LENGTH, -HALF_COLOR_LENGTH, -HALF_FLOAT_LENGTH);

        glColor3f(to_build ->colors.left.x,
                  to_build ->colors.left.y,
                  to_build ->colors.left.z);
        glVertex3f(HALF_COLOR_LENGTH, -HALF_FLOAT_LENGTH, HALF_COLOR_LENGTH);
        glVertex3f(-HALF_COLOR_LENGTH, -HALF_FLOAT_LENGTH, HALF_COLOR_LENGTH);
        glVertex3f(-HALF_COLOR_LENGTH, -HALF_FLOAT_LENGTH, -HALF_COLOR_LENGTH);
        glVertex3f(HALF_COLOR_LENGTH, -HALF_FLOAT_LENGTH, -HALF_COLOR_LENGTH);

        glColor3f(to_build ->colors.right.x,
                  to_build ->colors.right.y,
                  to_build ->colors.right.z);
        glVertex3f(HALF_COLOR_LENGTH, HALF_FLOAT_LENGTH, HALF_COLOR_LENGTH);
        glVertex3f(HALF_COLOR_LENGTH, HALF_FLOAT_LENGTH, -HALF_COLOR_LENGTH);
        glVertex3f(-HALF_COLOR_LENGTH, HALF_FLOAT_LENGTH, -HALF_COLOR_LENGTH);
        glVertex3f(-HALF_COLOR_LENGTH, HALF_FLOAT_LENGTH, HALF_COLOR_LENGTH);

        glColor3f(to_build ->colors.top.x,
                  to_build ->colors.top.y,
                  to_build ->colors.top.z);
        glVertex3f(-HALF_FLOAT_LENGTH, HALF_COLOR_LENGTH, HALF_COLOR_LENGTH);
        glVertex3f(-HALF_FLOAT_LENGTH, HALF_COLOR_LENGTH, -HALF_COLOR_LENGTH);
        glVertex3f(-HALF_FLOAT_LENGTH, -HALF_COLOR_LENGTH, -HALF_COLOR_LENGTH);
        glVertex3f(-HALF_FLOAT_LENGTH, -HALF_COLOR_LENGTH, HALF_COLOR_LENGTH);

        glColor3f(to_build ->colors.bottom.x,
                  to_build ->colors.bottom.y,
                  to_build ->colors.bottom.z);
        glVertex3f(HALF_FLOAT_LENGTH, HALF_COLOR_LENGTH, HALF_COLOR_LENGTH);
        glVertex3f(HALF_FLOAT_LENGTH, -HALF_COLOR_LENGTH, HALF_COLOR_LENGTH);
        glVertex3f(HALF_FLOAT_LENGTH, -HALF_COLOR_LENGTH, -HALF_COLOR_LENGTH);
        glVertex3f(HALF_FLOAT_LENGTH, HALF_COLOR_LENGTH, -HALF_COLOR_LENGTH);
    glEnd();
}

void MagicCubeGlWidget::Rotate_Face_Start(int face_color, int direction)
{
    temp_direction = direction;
    temp_face_color = face_color;
    key_operation_on = false;
    rotating = true;
    pass_rotation_angle = 0;
    flash_rotate_timer = new QTimer(this);
    connect(flash_rotate_timer, SIGNAL(timeout()), this, SLOT(Flash_Rotate_Play()));
    flash_rotate_timer ->start(5);
}

void MagicCubeGlWidget::pointer_change()
{
    int clock_sequence[8][2] =
    {
        {0, 0}, {0, 1}, {0, 2}, {1, 2},
        {2, 2}, {2, 1}, {2, 0}, {1, 0}
    };
    int_vector3 cube_sequence[8];
    //clock表示顺时针下标序列,cube是按旋转方向的一个方格下标序列
    SingleCube *temp_cube[8];
    int i, k;
    for (i = 0; i < 8; ++ i)
    {
        if (temp_direction * temp_face_color < 0)
            //temp_direction<0代表顺时针，temp_face_color<0代表左手系面
            k = i;
        else
            k = 7 - i;
        switch (temp_face_color)
        {
        case FC_YELLOW:
        case FC_WHITE:
            cube_sequence[i] = int_vector3(clock_sequence[k][0], clock_sequence[k][1],
                    (temp_face_color == FC_YELLOW ? 2 : 0));
            break;
        case FC_BLUE:
        case FC_GREEN:
            cube_sequence[i] = int_vector3((temp_face_color == FC_BLUE ? 2 : 0),
                                           clock_sequence[k][0], clock_sequence[k][1]);
            break;
        case FC_ORANGE:
        case FC_RED:
            cube_sequence[i] = int_vector3(clock_sequence[k][0],
                    (temp_face_color == FC_RED ? 2 : 0), clock_sequence[k][1]);
            break;
        }
    }
    //将方格下标按顺序排成一列
    for (i = 0; i < 8; ++ i)
        temp_cube[i] =
                cube_pointer_by_position[cube_sequence[i].x][cube_sequence[i].y][cube_sequence[i].z];
    for (i = 0; i < 8; ++ i)
    {
        int_vector3 temp_vector = cube_sequence[(i + 2) & 7];
        temp_cube[i] ->center_position = easy_vector3(temp_vector);
        temp_cube[i] ->rotate = Rotation();
        cube_pointer_by_position[temp_vector.x][temp_vector.y][temp_vector.z] = temp_cube[i];
        //接下来做颜色修正
        easy_vector3 temp_color;
        switch (temp_direction * temp_face_color)
        {
        case FC_YELLOW:
            temp_color = temp_cube[i] ->colors.left;
            temp_cube[i] ->colors.left = temp_cube[i] ->colors.top;
            temp_cube[i] ->colors.top = temp_cube[i] ->colors.right;
            temp_cube[i] ->colors.right = temp_cube[i] ->colors.bottom;
            temp_cube[i] ->colors.bottom = temp_color;
            break;
        case FC_WHITE:
            temp_color = temp_cube[i] ->colors.left;
            temp_cube[i] ->colors.left = temp_cube[i] ->colors.bottom;
            temp_cube[i] ->colors.bottom = temp_cube[i] ->colors.right;
            temp_cube[i] ->colors.right = temp_cube[i] ->colors.top;
            temp_cube[i] ->colors.top = temp_color;
            break;
        case FC_RED:
            temp_color = temp_cube[i] ->colors.face;
            temp_cube[i] ->colors.face = temp_cube[i] ->colors.top;
            temp_cube[i] ->colors.top = temp_cube[i] ->colors.back;
            temp_cube[i] ->colors.back = temp_cube[i] ->colors.bottom;
            temp_cube[i] ->colors.bottom = temp_color;
            break;
        case FC_ORANGE:
            temp_color = temp_cube[i] ->colors.face;
            temp_cube[i] ->colors.face = temp_cube[i] ->colors.bottom;
            temp_cube[i] ->colors.bottom = temp_cube[i] ->colors.back;
            temp_cube[i] ->colors.back = temp_cube[i] ->colors.top;
            temp_cube[i] ->colors.top = temp_color;
            break;
        case FC_GREEN:
            temp_color = temp_cube[i] ->colors.face;
            temp_cube[i] ->colors.face = temp_cube[i] ->colors.left;
            temp_cube[i] ->colors.left = temp_cube[i] ->colors.back;
            temp_cube[i] ->colors.back = temp_cube[i] ->colors.right;
            temp_cube[i] ->colors.right = temp_color;
            break;
        case FC_BLUE:
            temp_color = temp_cube[i] ->colors.face;
            temp_cube[i] ->colors.face = temp_cube[i] ->colors.right;
            temp_cube[i] ->colors.right = temp_cube[i] ->colors.back;
            temp_cube[i] ->colors.back = temp_cube[i] ->colors.left;
            temp_cube[i] ->colors.left = temp_color;
            break;
        }
    }
}
