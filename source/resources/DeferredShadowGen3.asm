// Summary: Improved shadow filter + restored normal offset bias + improved shadow fadeout
// Generated by Microsoft (R) HLSL Shader Compiler 9.26.952.2844
//
// Parameters:
//
//   sampler2D GBufferStencilTextureSampler;
//   sampler2D GBufferTextureSampler0;
//   sampler2D GBufferTextureSampler1;
//   sampler2D GBufferTextureSampler2;
//   sampler2D GBufferTextureSampler3;
//   sampler2D ParabSampler;
//   float4 dReflectionParams;
//   float4 gDeferredProjParams;
//   float4 gDirectionalColour;
//   float4 gDirectionalLight;
//   float4 gFacetCentre;
//   float4 gLightAmbient0;
//   float4 gLightAmbient1;
//   row_major float4x4 gShadowMatrix;
//   float4 gShadowParam0123;
//   float4 gShadowParam14151617;
//   float4 gShadowParam18192021;
//   float4 gShadowParam4567;
//   float4 gShadowParam891113;
//   sampler2D gShadowZSamplerDir;
//   row_major float4x4 gViewInverse;
//   float4 globalScalars;
//
//
// Registers:
//
//   Name                         Reg   Size
//   ---------------------------- ----- ----
//   gViewInverse                 c12      4
//   gDirectionalLight            c17      1
//   gDirectionalColour           c18      1
//   gLightAmbient0               c37      1
//   gLightAmbient1               c38      1
//   globalScalars                c39      1
//   gShadowParam18192021         c53      1
//   gFacetCentre                 c54      1
//   gShadowParam14151617         c56      1
//   gShadowParam0123             c57      1
//   gShadowParam4567             c58      1
//   gShadowParam891113           c59      1
//   gShadowMatrix                c60      4
//   gDeferredProjParams          c66      1
//   dReflectionParams            c72      1
//   GBufferTextureSampler0       s0       1
//   GBufferTextureSampler1       s1       1
//   GBufferTextureSampler2       s2       1
//   GBufferTextureSampler3       s4       1
//   ParabSampler                 s5       1
//   GBufferStencilTextureSampler s6       1
//   gShadowZSamplerDir           s15      1
//

    ps_3_0
    // def c219, 1.8395173895e+25, 3.9938258725e+24, 4.5435787456e+30, 8.4077907859e-45 // 6
    def c127, 0.9999999, 1, 0, 0	// LogDepth constants
    def c0, 512, 0.99609375, 7.96875, 63.75
    def c1, 0.25, 256, -127.999992, 9.99999975e-006
    def c2, 1.33333337, 9.99999975e-005, 512, 1
    def c3, 1, 0, 1.5, 0.0833333358
    def c4, -0.5, 0.5, 0.0199999996, 0.00999999978
    def c5, 4, 0.75, 0.25, 5
    def c6, 10, 0, 0, 0
    def c7, 1, -1, 0, -0
    def c8, -0.321940005, -0.932614982, -0.791558981, -0.597710013
    def c9, 0.507430971, 0.0644249991, 0.896420002, 0.412458003
    def c10, 0.519456029, 0.767022014, 0.185461, -0.893123984
    def c11, 0.962339997, -0.194983006, 0.473434001, -0.480026007
    def c12, -0.69591397, 0.457136989, -0.203345001, 0.620715976
    def c13, -0.326211989, -0.405809999, -0.840143979, -0.0735799968
	def c20, 1.6666667, 0, 0, 0	// Reflection intensity multiplier
	def c21, 3, 2, 1, 0	// Console tree lighting constants
	def c22, 0.012156862745098, 0.0007843137254902, 0, 0 // 3.1, 0.2
    def c24, 0.212500006, 0.715399981, 0.0720999986, 0.35
	// -------------------------------------------------------------- Shadow Constants --------------------------------------------------------------
    def c110, -0.25, 1, -1, 0
    def c111, 0.159154937, 0.5, 6.28318548, -3.14159274
    def c112, 3, 7.13800001, 0.00390625, 0
    def c113, 0.75, -0.5, 0.5, 1.5
	
    def c114, 0.14, 0.24, 0.5, 0.75 // static bias
    def c115, 0.24, 0.44, 1.1, 1.15
    def c116, 0.3, 0.54, 1.1, 0.95
    def c117, 0.64, 1.04, 2.2, 2.3
	
	def c118, 4, 0.3, 0, 0 // normal offset bias and blend params
	
	def c119, 0, 0.25, 0.5, 0.75 // UV clamps
	def c120, 0.2499, 0.4999, 0.7499, 1
	
	def c130, 0.18993645671348536, 0.027087114076591513, -0.21261242652069953, 0.23391293246949066
	def c131, 0.04771781344140756, -0.3666840644525993, 0.297730981239584, 0.398259878229082
	def c132, -0.509063425827436, -0.06528681462854097, 0.507855152944665, -0.2875976005206389
	def c133, -0.15230616564632418, 0.6426121151781916, -0.30240170651828074, -0.5805072900736001
	def c134, 0.6978019230005561, 0.2771173334141519, -0.6990963248129052, 0.3210960724922725
	def c135, 0.3565142601623699, -0.7066415061851589, 0.266890002328106, 0.8360191043249159
	def c136, -0.7515861305520581, -0.41609876195815027, 0.9102937449894895, -0.17014527555321657
	def c137, -0.5343471434373126, 0.8058593459499529, -0.1133270115046468, -0.9490025827627441
	// ----------------------------------------------------------------------------------------------------------------------------------------------
    dcl_texcoord v0.xy
    dcl_texcoord1 v1
    dcl_2d s0
    dcl_2d s1
    dcl_2d s2
    dcl_2d s4
    dcl_2d s5
	dcl_2d s6
    dcl_2d s10
    dcl_2d s15
    texld r1, v0, s1
    mul r2.xyz, r1.w, c0.yzww
    frc r2.xyz, r2
    add r3.xyz, r2, r2
    mad r3.xy, r2.yzzw, -c1.x, r3
    mad r1.xyz, r1, c1.y, r3
    add r1.xyz, r1, c1.z
    nrm r31.xyz, r1
    texld r0, v0, s4
	// ----------------------------------------------------------------- Log2Linear -----------------------------------------------------------------
	if_ne r0.x, c127.y
		rcp r20.x, c128.x
		mul r20.x, r20.x, c128.y
		pow r20.x, r20.x, r0.x
		mul r20.x, r20.x, c128.x	// W_clip
		
		add r20.y, r20.x, -c128.x
		add r20.z, c128.y, -c128.x
		mul r20.y, r20.y, c128.y
		mul r20.z, r20.z, r20.x
		rcp r20.z, r20.z
		mul r20.w, r20.y, r20.z		// Linear depth
		
		min r0, r20.w, c127.x		// FP error hack
	endif
	// ----------------------------------------------------------------------------------------------------------------------------------------------
    mad r0.x, r0.x, c66.z, -c66.w
    mul r0.x, r0.x, v1.w
    rcp r0.x, r0.x
    mov oC0.y, r0.x
    mov oC0.zw, c3.y
    mad r0.yzw, v1.xxyz, -r0.x, c15.xxyz
    dp3 r1.x, c14, r0.yzww
    add r1.xyz, -r1.x, -c54
    cmp r1.yzw, r1.xxyz, c3.x, c3.y
    mov r1.x, c2.w
    mad r21, r1, c110.yyyw, -r1.yzww // shadow cascade mask
    dp4 r2.x, r1, c57
    dp4 r2.y, r1, c58
    dp4 r3.x, r1, c59
    dp4 r3.y, r1, c56
    mul r1.xyz, r0.z, c61.xyww
    mad r1.xyz, r0.y, c60.xyww, r1
    mad r1.xyz, r0.w, c62.xyww, r1
    add r0.yzw, -r0, c15.xxyz
    dp3 r0.y, r0.yzww, r0.yzww
    rsq r0.y, r0.y
    rcp r0.y, r0.y
    add r1.xyz, r1, c63.xyww
    mul r20.xyz, r31.y, c61.xyww
    mad r20.xyz, r31.x, c60.xyww, r20
    mad r20.xyz, r31.z, c62.xyww, r20
    // ---------------------------------------------------------------- Shadow Fixes -----------------------------------------------------------------
    add r22, c54.w, -c54
    add r22, c53.w, -r22 // cascade ranges
    dp4 r23.x, r21_abs, r22
    dp4 r23.y, r21_abs, r22.yzww
    
    rcp r23.z, r22.x
    mul r23.zw, r23.xyxy, r23.z // (curr_range, next_range) / first_range
    mul r24.x, r22.x, c53.y
    mul r24.x, r24.x, c118.x
    mul r23.zw, r23, r24.x // bias magnitude
    
    mad r24, r22.xxyz, -c110.wyyy, r22
    dp4 r24.x, r21_abs, r24 // curr_range - prev_range
    mul r24.x, r24.x, c218.z
    rcp r24.z, r24.x
    add r24.x, r23.x, -r24.x
    add r24.x, r0.x, -r24.x
    mul_sat r24.x, r24.x, r24.z // blending factor
    
    rcp r24.z, r23.x
    mul r23.xy, r23.xy, r24.z // 1.0, next_range / curr_range
    
    lrp r22.xy, r24.x, r23.yw, r23.xz
    
    m4x4 r24, r21_abs, c114
    dp4 r24.x, r24, c220
    add r1.z, r1.z, -r24.x // undo static bias
    
    mul r22.y, r22.y, c223.y // scale bias with FOV
    mul r22.y, r22.y, c218.y
    mad r1.xyz, r20, r22.y, r1 // normal offset bias
    
    mul r20.xy, r22.x, c53.y
    rcp r20.x, c58.x
    mul r20.x, r20.x, c57.x
    mul r20.x, r20.x, r20.y // fix filter stretching
    
    mul r20.xy, r20.xy, c218.x
    
    dp4 r20.z, r21_abs, c119 // fix pixels leaking into other cascades
    dp4 r20.w, r21_abs, c120
    // ----------------------------------------------------------------------------------------------------------------------------------------------
    mad r0.zw, r1.xyxy, r2.xyxy, r3.xyxy
	// ---------------------------------------------------------- Improved Shadow Filter ------------------------------------------------------------
    mul r20.xy, r20.xy, c113.w // blur factor
    
	mul r24.xy, c44.xy, v0.xy
	mul r24.xy, r24.xy, c112.z
	texldl r24, r24, s10
	mul r24.x, r24.z, c111.z
	sincos r25.xy, r24.x
	mul r25, r25.xyyx, c110.yzyy
	mul r26, c130.xyxy, r25
	add r27.xy, r26.xzxz, r26.ywyw
	mul r26, c130.zwzw, r25
	add r27.zw, r26.xzxz, r26.ywyw
	mad r26, r27, r20.xyxy, r0.zwzw
	max r26.xz, r26, r20.z
	min r26.xz, r26, r20.w
	texldl r28, r26.xy, s15
	texldl r27, r26.zw, s15
	mov r28.y, r27.x
	mul r26, c131.xyxy, r25
	add r27.xy, r26.xzxz, r26.ywyw
	mul r26, c131.zwzw, r25
	add r27.zw, r26.xzxz, r26.ywyw
	mad r26, r27, r20.xyxy, r0.zwzw
	max r26.xz, r26, r20.z
	min r26.xz, r26, r20.w
	texldl r27, r26.xy, s15
	mov r28.z, r27.x
	texldl r27, r26.zw, s15
	mov r28.w, r27.x
	add r28, r1.z, -r28
	cmp r28, r28, c110.y, c110.w
	dp4 r29.x, r28, -c110.x
	mul r26, c132.xyxy, r25
	add r27.xy, r26.xzxz, r26.ywyw
	mul r26, c132.zwzw, r25
	add r27.zw, r26.xzxz, r26.ywyw
	mad r26, r27, r20.xyxy, r0.zwzw
	max r26.xz, r26, r20.z
	min r26.xz, r26, r20.w
	texldl r28, r26.xy, s15
	texldl r27, r26.zw, s15
	mov r28.y, r27.x
	mul r26, c133.xyxy, r25
	add r27.xy, r26.xzxz, r26.ywyw
	mul r26, c133.zwzw, r25
	add r27.zw, r26.xzxz, r26.ywyw
	mad r26, r27, r20.xyxy, r0.zwzw
	max r26.xz, r26, r20.z
	min r26.xz, r26, r20.w
	texldl r27, r26.xy, s15
	mov r28.z, r27.x
	texldl r27, r26.zw, s15
	mov r28.w, r27.x
	add r28, r1.z, -r28
	cmp r28, r28, c110.y, c110.w
	dp4 r29.y, r28, -c110.x
	mul r26, c134.xyxy, r25
	add r27.xy, r26.xzxz, r26.ywyw
	mul r26, c134.zwzw, r25
	add r27.zw, r26.xzxz, r26.ywyw
	mad r26, r27, r20.xyxy, r0.zwzw
	max r26.xz, r26, r20.z
	min r26.xz, r26, r20.w
	texldl r28, r26.xy, s15
	texldl r27, r26.zw, s15
	mov r28.y, r27.x
	mul r26, c135.xyxy, r25
	add r27.xy, r26.xzxz, r26.ywyw
	mul r26, c135.zwzw, r25
	add r27.zw, r26.xzxz, r26.ywyw
	mad r26, r27, r20.xyxy, r0.zwzw
	max r26.xz, r26, r20.z
	min r26.xz, r26, r20.w
	texldl r27, r26.xy, s15
	mov r28.z, r27.x
	texldl r27, r26.zw, s15
	mov r28.w, r27.x
	add r28, r1.z, -r28
	cmp r28, r28, c110.y, c110.w
	dp4 r29.z, r28, -c110.x
	mul r26, c136.xyxy, r25
	add r27.xy, r26.xzxz, r26.ywyw
	mul r26, c136.zwzw, r25
	add r27.zw, r26.xzxz, r26.ywyw
	mad r26, r27, r20.xyxy, r0.zwzw
	max r26.xz, r26, r20.z
	min r26.xz, r26, r20.w
	texldl r28, r26.xy, s15
	texldl r27, r26.zw, s15
	mov r28.y, r27.x
	mul r26, c137.xyxy, r25
	add r27.xy, r26.xzxz, r26.ywyw
	mul r26, c137.zwzw, r25
	add r27.zw, r26.xzxz, r26.ywyw
	mad r26, r27, r20.xyxy, r0.zwzw
	max r26.xz, r26, r20.z
	min r26.xz, r26, r20.w
	texldl r27, r26.xy, s15
	mov r28.z, r27.x
	texldl r27, r26.zw, s15
	mov r28.w, r27.x
	add r28, r1.z, -r28
	cmp r28, r28, c110.y, c110.w
	dp4 r29.w, r28, -c110.x
	dp4 r20.x, r29, -c110.x
	// ----------------------------------------------------------------------------------------------------------------------------------------------
    // -------------------------------------------------------------- Improved Fadeout --------------------------------------------------------------
    rcp r20.y, c53.w
    mul_sat r20.y, r20.y, r0.y
    mul r20.y, r20.y, r20.y
    lrp oC0.x, r20.y, c110.y, r20.x // improved fadeout
	// ----------------------------------------------------------------------------------------------------------------------------------------------

// approximately 167 instruction slots used (18 texture, 149 arithmetic)
