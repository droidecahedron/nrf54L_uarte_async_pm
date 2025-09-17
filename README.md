# nrf54L_uarte_async_pm

## Requirements
### Software
nRF Connect SDK `v3.1.0`

### Hardware
nRF54L15-DK

## Description 

This sample will use the console uart to echo your message.
Every 2 seconds, it alternates between the UART peripheral being active or inactive.

While active, the current draw will be higher but anything you send over the UART interface with the DK (VCOM1) will be echoed back to you.
While inactive, it will have RX disabled and the peripheral powered down, so anything you do won't be detected.

## Example log output
```
*** Booting nRF Connect SDK v3.1.0-6c6e5b32496e ***
*** Using Zephyr OS v4.1.99-1612683d4010 ***
> asdadad <-- ignored since uart inactive
> asadad
asadad <-- echoed since uart active
> asdsad <-- ignored since uart inactive
> asdsadasd
asdsadsd <-- echoed since uart active
```

## Example power measurements
### UART Inactive
<img width="506" height="467" alt="image" src="https://github.com/user-attachments/assets/ffb269b5-5323-412f-bc55-9c7c70515648" />

### UART Active
<img width="506" height="467" alt="image" src="https://github.com/user-attachments/assets/7b0336a1-042a-4fb2-94d2-47cce613a23b" />
