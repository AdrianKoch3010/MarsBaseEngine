# Game AI


## Main Players

<table>
    <tr>
        <th>AIState</th>
        <th>AIComponent</th>
        <th>AISystem</th>
    </tr>
    <tr>
        <td>
        <ul>
            <li>Knowledge of the game world</li>
            <li>Knowledge of the entity</li>
        </ul>
        </td>
        <td>
        <ul>
            <li>Manages AI states</li>
            <li>Provides interface for adding / removing states</li>
        </ul>
        </td>
        <td>
        <ul>
            <li>Dedicated to a specific area of AI</li>
            <li>Manages transition logic between AI states through the interface provided by the AI components</li>
        </ul>
        </td>
    </tr>
</table> 