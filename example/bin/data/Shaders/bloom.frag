//https://www.shadertoy.com/view/lsXGWn#

uniform vec3      iResolution;           // viewport resolution (in pixels)
uniform float     iGlobalTime;           // shader playback time (in seconds)
uniform float     iChannelTime[4];       // channel playback time (in seconds)
uniform vec3      iChannelResolution[4]; // channel resolution (in pixels)
uniform vec4      iMouse;                // mouse pixel coords. xy: current (if MLB down), zw: click
uniform sampler2D iChannel0;          // input channel. XX = 2D/Cube
uniform vec4      iDate;                 // (year, month, day, time in seconds)


uniform float intensity;
      
const float blurSize = 1.0/512.0;


void main()
{
    
    vec4 sum = vec4(0);
    vec2 uv = gl_FragCoord.xy/iResolution.xy;
    int j;
    int i;
    
    //thank you! http://www.gamerendering.com/2008/10/11/gaussian-blur-filter-shader/ for the
    //blur tutorial
    // blur in y (vertical)
    // take nine samples, with the distance blurSize between them
    sum += texture2D(iChannel0, vec2(uv.x - 4.0*blurSize, uv.y)) * 0.05;
    sum += texture2D(iChannel0, vec2(uv.x - 3.0*blurSize, uv.y)) * 0.09;
    sum += texture2D(iChannel0, vec2(uv.x - 2.0*blurSize, uv.y)) * 0.12;
    sum += texture2D(iChannel0, vec2(uv.x - blurSize, uv.y)) * 0.15;
    sum += texture2D(iChannel0, vec2(uv.x, uv.y)) * 0.16;
    sum += texture2D(iChannel0, vec2(uv.x + blurSize, uv.y)) * 0.15;
    sum += texture2D(iChannel0, vec2(uv.x + 2.0*blurSize, uv.y)) * 0.12;
    sum += texture2D(iChannel0, vec2(uv.x + 3.0*blurSize, uv.y)) * 0.09;
    sum += texture2D(iChannel0, vec2(uv.x + 4.0*blurSize, uv.y)) * 0.05;
	
	// blur in y (vertical)
    // take nine samples, with the distance blurSize between them
    sum += texture2D(iChannel0, vec2(uv.x, uv.y - 4.0*blurSize)) * 0.05;
    sum += texture2D(iChannel0, vec2(uv.x, uv.y - 3.0*blurSize)) * 0.09;
    sum += texture2D(iChannel0, vec2(uv.x, uv.y - 2.0*blurSize)) * 0.12;
    sum += texture2D(iChannel0, vec2(uv.x, uv.y - blurSize)) * 0.15;
    sum += texture2D(iChannel0, vec2(uv.x, uv.y)) * 0.16;
    sum += texture2D(iChannel0, vec2(uv.x, uv.y + blurSize)) * 0.15;
    sum += texture2D(iChannel0, vec2(uv.x, uv.y + 2.0*blurSize)) * 0.12;
    sum += texture2D(iChannel0, vec2(uv.x, uv.y + 3.0*blurSize)) * 0.09;
    sum += texture2D(iChannel0, vec2(uv.x, uv.y + 4.0*blurSize)) * 0.05;
    
    //increase blur with intensity!
    vec4 tex0 = (sum*intensity)+texture2D(iChannel0,uv);

    vec4 color = tex0;
	gl_FragColor = vec4( color);
    
}