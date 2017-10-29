precision mediump float;

uniform sampler2D uTexture;

varying vec2 vTexCoord;
varying vec4 vVertColor;

void main()
{
	gl_FragColor = texture2D( uTexture, vTexCoord ) * vVertColor;
}