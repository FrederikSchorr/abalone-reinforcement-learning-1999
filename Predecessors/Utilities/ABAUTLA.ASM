  DOSSEG
  .MODEL small
  .STACK 100h
  .DATA
  .CODE
GLOBAL _testmpix





;*****
;_testmpix
;*****
;
;testet einen pixel auf eine bestimmte farbe wobei maus ausgeschalten wird
;
;aufruf aus C: testmpix (int x,int y,byte farbe)
;output: AX ( = returnwert): 0 => nein
;zerstort: AX (return),BX,CX,DX = 03CEh
;

testpix	STRUC
x	DW ?
y	DW ?
farbe	DB ?
testpix	ENDS


_testmpix PROC

  push bp
  mov bp,sp
  tpix EQU [bp + 4]


  mov dx,80
  mov ax,tpix.y
  mul dx                        ;y * 640 / 8
  mov bx,tpix.x
  shr bx,1
  shr bx,1
  shr bx,1                      ;x / 8
  add bx,ax                     ;bx - Offset
  mov cx,tpix.x
  and cl,7
  xor cl,7
  mov ch,1
  shl ch,cl                     ;ch - Maske

  mov ax,0A000h                 ;vioseg
  mov es,ax
  mov dx,03CEh
  mov ax,0002h                  ;
  int 33h                       ;maus ausblenden
  mov ax,0805h                	;read1
  out dx,ax
  mov al,02h
  mov ah,tpix.farbe             ;farbe
  out dx,ax

  mov cl,es:[bx]                ;vrambyte in cl
  mov ax,0001h			;
  int 33h			;maus einblenden (& restaurieren)

  mov al,ch
  and al,cl                     ;maske & byte in ax

  pop bp
  ret
_testmpix ENDP










  END

