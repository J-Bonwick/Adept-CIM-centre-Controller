# SCP - Serial Command Protocol

SCP is the primary way of interfacing with the this CNC controller.


TODO:
Get End stops as bit array
Get status lines as bit array
Get front panel buttons as bit array

## Commands_List
| Command              | Syntax                  |
|:---------------------|:------------------------|
| Help                 | `H`                     |
| Info                 | `I`                     |
| Axis Position Set    | `APS <AXIS> <POS>`      |
| Axis Position Get    | `APG <AXIS>`            |
| ?Axis State          | `AS <AXIS>`             |
| Spindle Speed Set    | `SSS <SPINDLE> <SPEED>` |
| Spindle Speed Get    | `SSG <SPINDLE>`         |
| Spindle Position Get | `SPG <SPINDLE>`         |
| ?Spindle State       | `SS <SPINDLE>`          |


## Command `H`:
Prints a list of available commands with a brief description of each.

Command: `H`
Args: 'NONE'

## Verion `V`:
Returns the current software version.

Command: 'version'
Args: 'NONE'

## Axis Position Set : `APS <AXIS> <POSITION>`:
Sets the specified axis position.

### Args:
#### `<AXIS>`
Supported axis:
- `FB` : Forward-Backward
- `LR` : Left-Right
- `UD` : Up-Down

### Example: 
Set axis FB to 200
`AS FB 200`
