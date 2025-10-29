## Report for laboration 4 - Computer Technology 1

  - **My name and LNU credentials:** Oscar Ekberg, oe222ia
  - **Examiner of this report**: Daniel Lundberg

## Task 1 - Hello World loop
# Minicom output
<img width="650" height="442" alt="image" src="https://github.com/user-attachments/assets/7f71df8f-03cc-4320-bb05-b09e126e09d5" />

# Source code
```assembly
@
@ Assembler program print out "Hello World"
@ using the Pico SDK.
@
@ R0 - first parameter to printf
@ R1 - second parameter to printer
@ R7 - index counter
@

.thumb_func		    @ Necessary because sdk uses BLX
.global main	            @ Provide program starting address to linker

main:
	BL	stdio_init_all	@ initialize uart or usb
	MOVS	R7, #100		@ initialize starting counter to 100
loop:
	LDR	R0, =helloworld	@ load address of helloworld string
	MOV R1, R7 @ Moves the counter in to R1 which is sent to the printf function
	BL	printf		@ Call pico_printf
	SUB R7, R7, #1 @ Subtracts 1 from R7
	CMP R7, #0 @ Compares R7 to 0
	BGE loop @ Branch if Greater than or Equal to 0
	MOVS R7, #100 @ Resets the counter to 100
	B	loop		@ loop forever
		
.data
	       .align  4	@ necessary alignment
helloworld: .asciz   "Hello World %d\n"

```

## Task 2 - Traffic light
# Output
https://github.com/user-attachments/assets/a4c00085-14e7-4cb0-b4df-dd693df4b2ac

# Source code
```assembly
@
@ Assembler program to flash three LEDs connected to the
@ Raspberry Pi Pico GPIO port using the Pico SDK.
@

	.EQU	LED_PIN1, 0
	.EQU	LED_PIN2, 1
	.EQU	LED_PIN3, 2
	.EQU	GPIO_OUT, 1
	.EQU	sleep_time, 200

.thumb_func	@ Necessary because sdk uses BLX
.global main    @ Provide program starting address

main:
	BL stdio_init_all @ enable USB serial
	
	MOV	R0, #LED_PIN1 @ Puts the LED_PIN1 varaible in to R0, the parameter
	BL	gpio_init @ Initializes pin
	MOV	R0, #LED_PIN1
	MOV	R1, #GPIO_OUT @ Puts the GPIO_OUT variable in to R1, the second parameter
	BL	link_gpio_set_dir @ Calls the externally imported C function
	
	MOV	R0, #LED_PIN2
	BL	gpio_init
	MOV	R0, #LED_PIN2
	MOV	R1, #GPIO_OUT
	BL	link_gpio_set_dir
	
	MOV	R0, #LED_PIN3
	BL	gpio_init
	MOV	R0, #LED_PIN3
	MOV	R1, #GPIO_OUT
	BL	link_gpio_set_dir
	
loop:
	MOV	R0, #LED_PIN1
	MOV	R1, #1 @ Assigns the value 1 to Register 1 (R1)
	BL	link_gpio_put @ Calls the C function
	LDR	R0, =sleep_time @ Loads the value which the program will sleep
	BL	sleep_ms @ Program sleeps for 200ms
	MOV	R0, #LED_PIN1
	MOV	R1, #0
	BL	link_gpio_put
	MOV	R0, #LED_PIN2
	MOV	R1, #1
	BL	link_gpio_put
	LDR	R0, =sleep_time
	BL	sleep_ms
	MOV	R0, #LED_PIN2
	MOV	R1, #0
	BL	link_gpio_put
	MOV	R0, #LED_PIN3
	MOV	R1, #1
	BL	link_gpio_put
	LDR	R0, =sleep_time
	BL	sleep_ms
	MOV	R0, #LED_PIN3
	MOV	R1, #0
	BL	link_gpio_put
	B       loop @ Loop continues infinitely

```

## Task 3 - Binary counter
# Output
**Link to Google Drive due to file exceeding accepted size**

https://drive.google.com/file/d/1TvsntBlSTeEqIup9cvoACHtylR23L0ED/view?usp=drive_link

# Source code
```assembly
@
@ Assembler program to flash three LEDs that count in binary form from 0 to 7 and then from 7 to 0 infinitely
@ Raspberry Pi Pico GPIO port using the Pico SDK.
@

	.EQU	LED_PIN1, 0 @green
	.EQU	LED_PIN2, 1 @yellow
	.EQU	LED_PIN3, 2 @red
	.EQU	GPIO_OUT, 1
	.EQU	sleep_time, 1000

.thumb_func	@ Necessary because sdk uses BLX
.global main    @ Provide program starting address

main:
	BL stdio_init_all @ enable USB serial
@Initializing LEDs
	MOV	R0, #LED_PIN1
	BL	gpio_init
	MOV	R0, #LED_PIN1
	MOV	R1, #GPIO_OUT
	BL	link_gpio_set_dir
	
	MOV	R0, #LED_PIN2
	BL	gpio_init
	MOV	R0, #LED_PIN2
	MOV	R1, #GPIO_OUT
	BL	link_gpio_set_dir
	
	MOV	R0, #LED_PIN3
	BL	gpio_init
	MOV	R0, #LED_PIN3
	MOV	R1, #GPIO_OUT
	BL	link_gpio_set_dir

@ Making sure all LEDs are off before initializing loop
	MOV R0, #LED_PIN1
    MOV R1, #0
    BL link_gpio_put

    MOV R0, #LED_PIN2
    MOV R1, #0
    BL link_gpio_put

    MOV R0, #LED_PIN3
    MOV R1, #0
    BL link_gpio_put

loop:
@Counting from 0 to 7
@0
	MOV R0, #LED_PIN1
	MOV R1, #0
	BL link_gpio_put
	LDR R0, =sleep_time
	BL sleep_ms
@1
	MOV R0, #LED_PIN1
	MOV R1, #1
	BL link_gpio_put
	LDR R0, =sleep_time
	BL sleep_ms
	
	MOV R0, #LED_PIN1
	MOV R1, #0
	BL link_gpio_put
@2
	MOV R0, #LED_PIN2
	MOV R1, #1
	BL link_gpio_put
	LDR R0, =sleep_time
	BL sleep_ms
	
	MOV R0, #LED_PIN2
	MOV R1, #0
	BL link_gpio_put
@3
	MOV R0, #LED_PIN1
	MOV R1, #1
	BL link_gpio_put
	
	MOV R0, #LED_PIN2
	MOV R1, #1
	BL link_gpio_put
	LDR R0, =sleep_time
	BL sleep_ms
	
	MOV R0, #LED_PIN1
	MOV R1, #0
	BL link_gpio_put
	
	MOV R0, #LED_PIN2
	MOV R1, #0
	BL link_gpio_put
@4
	MOV R0, #LED_PIN3
	MOV R1, #1
	BL link_gpio_put
	LDR R0, =sleep_time
	BL sleep_ms
	
	MOV R0, #LED_PIN3
	MOV R1, #0
@5
	MOV R0, #LED_PIN1
	MOV R1, #1
	BL link_gpio_put
	
	MOV R0, #LED_PIN3
	MOV R1, #1
	BL link_gpio_put
	LDR R0, =sleep_time
	BL sleep_ms
	
	MOV R0, #LED_PIN1
	MOV R1, #0
	BL link_gpio_put
	
	MOV R0, #LED_PIN3
	MOV R1, #0
	BL link_gpio_put
@6
	MOV R0, #LED_PIN2
	MOV R1, #1
	BL link_gpio_put
	
	MOV R0, #LED_PIN3
	MOV R1, #1
	BL link_gpio_put
	LDR R0, =sleep_time
	BL sleep_ms
	
	MOV R0, #LED_PIN2
	MOV R1, #0
	BL link_gpio_put
	
	MOV R0, #LED_PIN3
	MOV R1, #0
	BL link_gpio_put
@7
	MOV R0, #LED_PIN1
	MOV R1, #1
	BL link_gpio_put
	
	MOV R0, #LED_PIN2
	MOV R1, #1
	BL link_gpio_put
	
	MOV R0, #LED_PIN3
	MOV R1, #1
	BL link_gpio_put
	LDR R0, =sleep_time
	BL sleep_ms
	
	MOV R0, #LED_PIN1
	MOV R1, #0
	BL link_gpio_put
	
	MOV R0, #LED_PIN2
	MOV R1, #0
	BL link_gpio_put
	
	MOV R0, #LED_PIN3
	MOV R1, #0
	BL link_gpio_put

@Now counting from 7 to 0

@6
	MOV R0, #LED_PIN2
	MOV R1, #1
	BL link_gpio_put
	
	MOV R0, #LED_PIN3
	MOV R1, #1
	BL link_gpio_put
	LDR R0, =sleep_time
	BL sleep_ms
	
	MOV R0, #LED_PIN2
	MOV R1, #0
	BL link_gpio_put
	
	MOV R0, #LED_PIN3
	MOV R1, #0
	BL link_gpio_put
@5
	MOV R0, #LED_PIN1
	MOV R1, #1
	BL link_gpio_put
	
	MOV R0, #LED_PIN3
	MOV R1, #1
	BL link_gpio_put
	LDR R0, =sleep_time
	BL sleep_ms
	
	MOV R0, #LED_PIN1
	MOV R1, #0
	BL link_gpio_put
	
	MOV R0, #LED_PIN3
	MOV R1, #0
	BL link_gpio_put
@4
	MOV R0, #LED_PIN3
	MOV R1, #1
	BL link_gpio_put
	LDR R0, =sleep_time
	BL sleep_ms
	
	MOV R0, #LED_PIN3
	MOV R1, #0
	BL link_gpio_put
@3
	MOV R0, #LED_PIN1
	MOV R1, #1
	BL link_gpio_put
	
	MOV R0, #LED_PIN2
	MOV R1, #1
	BL link_gpio_put
	LDR R0, =sleep_time
	BL sleep_ms
	
	MOV R0, #LED_PIN1
	MOV R1, #0
	BL link_gpio_put
	
	MOV R0, #LED_PIN2
	MOV R1, #0
	BL link_gpio_put
@2
	MOV R0, #LED_PIN2
	MOV R1, #1
	BL link_gpio_put
	LDR R0, =sleep_time
	BL sleep_ms
	
	MOV R0, #LED_PIN2
	MOV R1, #0
	BL link_gpio_put
@1
	MOV R0, #LED_PIN1
	MOV R1, #1
	BL link_gpio_put
	LDR R0, =sleep_time
	BL sleep_ms
	
	MOV R0, #LED_PIN1
	MOV R1, #0
	BL link_gpio_put
	B 		loop
```

## Task 4 - 7-segment display
# Output
**Link to Google Drive due to file exceeding accepted size**

https://drive.google.com/file/d/1HULi1SrScvfnbIuCinBTfCoerD3l0S-Y/view?usp=sharing

# Source code
```assembly
@
@ Assembler program to flash three LEDs that count in binary form from 0 to 7 and then from 7 to 0 infinitely
@ Raspberry Pi Pico GPIO port using the Pico SDK.
@

	.EQU	LED_PIN1, 0
	.EQU	LED_PIN2, 1
	.EQU	LED_PIN3, 2
	.EQU	LED_PIN4, 3
	.EQU	LED_PIN5, 4
	.EQU	LED_PIN6, 5
	.EQU	LED_PIN7, 6
	.EQU	GPIO_OUT, 1
	.EQU	sleep_time, 1000

.thumb_func	@ Necessary because sdk uses BLX
.global main    @ Provide program starting address

main:
	BL stdio_init_all @ enable USB serial

@Initializing LEDs
	MOV	R0, #LED_PIN1
	BL	gpio_init
	MOV	R0, #LED_PIN1
	MOV	R1, #GPIO_OUT
	BL	link_gpio_set_dir
	
	MOV	R0, #LED_PIN2
	BL	gpio_init
	MOV	R0, #LED_PIN2
	MOV	R1, #GPIO_OUT
	BL	link_gpio_set_dir
	
	MOV	R0, #LED_PIN3
	BL	gpio_init
	MOV	R0, #LED_PIN3
	MOV	R1, #GPIO_OUT
	BL	link_gpio_set_dir
	
	@Initializing LEDs
	MOV	R0, #LED_PIN4
	BL	gpio_init
	MOV	R0, #LED_PIN4
	MOV	R1, #GPIO_OUT
	BL	link_gpio_set_dir
	
	MOV	R0, #LED_PIN5
	BL	gpio_init
	MOV	R0, #LED_PIN5
	MOV	R1, #GPIO_OUT
	BL	link_gpio_set_dir
	
	MOV	R0, #LED_PIN6
	BL	gpio_init
	MOV	R0, #LED_PIN6
	MOV	R1, #GPIO_OUT
	BL	link_gpio_set_dir
	
	MOV	R0, #LED_PIN7
	BL	gpio_init
	MOV	R0, #LED_PIN7
	MOV	R1, #GPIO_OUT
	BL	link_gpio_set_dir

loop:	
@0
	MOV R0, #63 @ Puts the value 64 in to Register 0 (R0)
	BL link_gpio_put_all @ Lights the segments for displaying 0
	LDR R0, =sleep_time @ The sleep value is loaded in to Register R0
	BL sleep_ms @ Program sleeps
@1
@0000110
	MOV R0, #6
	BL link_gpio_put_all
	LDR R0, =sleep_time
	BL sleep_ms
@2
@1011011
	MOV R0,#91
	BL link_gpio_put_all
	LDR R0, =sleep_time
	BL sleep_ms
@3
@1001111
	MOV R0, #79
	BL link_gpio_put_all
	LDR R0, =sleep_time
	BL sleep_ms
@4
@1100110
	MOV R0, #102
	BL link_gpio_put_all
	LDR R0, =sleep_time
	BL sleep_ms
@5
@1101101
	MOV R0, #109
	BL link_gpio_put_all
	LDR R0, =sleep_time
	BL sleep_ms
@6
@1111101
	MOV R0, #125
	BL link_gpio_put_all
	LDR R0, =sleep_time
	BL sleep_ms
@7
@0000111
	MOV R0, #7
	BL link_gpio_put_all
	LDR R0, =sleep_time
	BL sleep_ms
@8
@1111111
	MOV R0, #127
	BL link_gpio_put_all
	LDR R0, =sleep_time
	BL sleep_ms
@9
@1100111
	MOV R0, #111
	BL link_gpio_put_all
	LDR R0, =sleep_time
	BL sleep_ms
@8
@1111111
	MOV R0, #127
	BL link_gpio_put_all
	LDR R0, =sleep_time
	BL sleep_ms
@7
@0000111
	MOV R0, #7
	BL link_gpio_put_all
	LDR R0, =sleep_time
	BL sleep_ms
@6
@1111101
	MOV R0, #125
	BL link_gpio_put_all
	LDR R0, =sleep_time
	BL sleep_ms
@5
@1101101
	MOV R0, #109
	BL link_gpio_put_all
	LDR R0, =sleep_time
	BL sleep_ms
@4
@1100110
	MOV R0, #102
	BL link_gpio_put_all
	LDR R0, =sleep_time
	BL sleep_ms
@3
@1001111
	MOV R0, #79
	BL link_gpio_put_all
	LDR R0, =sleep_time
	BL sleep_ms
@2
@1011011
	MOV R0,#91
	BL link_gpio_put_all
	LDR R0, =sleep_time
	BL sleep_ms
@1
@0000110
	MOV R0, #6
	BL link_gpio_put_all
	LDR R0, =sleep_time
	BL sleep_ms
B 	loop

```
