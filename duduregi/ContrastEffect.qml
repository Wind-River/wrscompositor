import QtQuick 2.0

ShaderEffect {
    property variant source: null;
    property color color: "#ffffff"
    property real blend;

    onSourceChanged: {
        if (source != null) {
            source.setPaintEnabled(false);
        }
    }

    property string vShaderInvertedY: "
    uniform highp mat4 qt_Matrix;
    attribute highp vec4 qt_Vertex;
    attribute highp vec2 qt_MultiTexCoord0;
    varying highp vec2 qt_TexCoord0;
    void main() {
        qt_TexCoord0 = qt_MultiTexCoord0;
        gl_Position = qt_Matrix * qt_Vertex;
    }
    "
    property string vShader: "
    uniform highp mat4 qt_Matrix;
    attribute highp vec4 qt_Vertex;
    attribute highp vec2 qt_MultiTexCoord0;
    varying highp vec2 qt_TexCoord0;
    void main() {
        qt_TexCoord0 = vec2(0, 1) + qt_MultiTexCoord0 * vec2(1, -1);
        gl_Position = qt_Matrix * qt_Vertex;
    }
    "

    vertexShader: source && source.isYInverted ? vShaderInvertedY : vShader

    fragmentShader: "
    uniform sampler2D source;
    uniform float qt_Opacity;
    uniform vec4 color;
    uniform float blend;
    varying highp vec2 qt_TexCoord0;
    void main() {
        vec4 sourceColor = texture2D(source, qt_TexCoord0);
        vec3 delta = sourceColor.rgb - vec3(0.5);
        vec3 lowerContrast = vec3(0.5) + 0.4 * delta;
        gl_FragColor = qt_Opacity * mix(sourceColor, color * sourceColor.a * dot(lowerContrast, vec3(11, 16, 5) * (1. /  32.)), blend);
    }
    "
}
