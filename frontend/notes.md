# Frontend Size

### Plain React app

| File       | Size  |
| ---------- | ----- |
| index.html | 0.46K |
| index.css  | 0.35K |
| index.js   | 139K  |
| Total      | 140K  |

### React app with tailwind

| File       | Size  |
| ---------- | ----- |
| index.html | 0.46K |
| index.css  | 5.10K |
| index.js   | 139K  |
| Total      | 145K  |

### React app with tailwind

| File       | Size  |
| ---------- | ----- |
| index.html | 0.46K |
| index.css  | 5.60K |
| index.js   | 147K  |
| Total      | 153K  |


## Calculate CanMessage

### CanData
```json
{
    "id": 568, 
    "length": 8, 
    "data": [39, 16, 0, 255, 39, 16, 39, 16], 
    "isRTR": 0, 
    "isExtended": 0, 
    "frequency": 98.73 
}
```

### DBC
```json
{
    "name": "CHT1",
    "startBit": 7,
    "length": 16,
    "isUnsigned": false,
    "isLittleEndian": false,
    "scale": 0.1,
    "offset": 0,
    "unit": "°C"
}
```

$39$ in binary -> $0010 0111$<br>
$16$ in binary -> $0001 0000$<br>
$0$ in binary -> $0010 0111$<br>
$16$ in binary -> $0001 0000$<br>
$39$ in binary -> $0010 0111$<br>
$16$ in binary -> $0001 0000$<br>

