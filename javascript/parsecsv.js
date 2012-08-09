//  This code is from here -  
//  http://lach.la/n/2011/12/15/25/csv-parser-in-javascript
//  Many thanks to Lachlan Arthur for doing this!  
//  He says (on the above page) -  
//  "License: Do whatever you want with it, public domain :D  "  

function parseCSV(rawData) {
	var rawRows = rawData.split("\n");
	var csv = [];
	var row = [];
	for (var rowIndex = 0; rowIndex < rawRows.length; rowIndex++) {
		var currentRow = rawRows[rowIndex].split(",");
		var cell = "";
		var in_quotes = false;
		for (var cellIndex = 0; cellIndex < currentRow.length; cellIndex++) {
			var currentCell = currentRow[cellIndex];
			var quote_start = currentCell.search('"') == 0; // Starts with a quote
			var quote_end = currentCell.substr(1).search('"') == currentCell.length - 2; // Ends with a quote
			if (quote_start && quote_end) { // The cell starts and ends with quotes. It's a single cell.
				cell = currentCell.substr(1, currentCell.length - 2);
			} else if (quote_start) { // The cell is the beginning of several values.
				in_quotes = true;
				cell = currentCell.substr(1) + ",";
			} else if (quote_end) { // The cell is the end of several values.
				in_quotes = false;
				cell += currentCell.substr(0, currentCell.length - 1);
			} else if (in_quotes) { // The cell is in between the start and end quoted cells.
				cell += currentCell + ",";
			} else { // Just a regular, unquoted cell.
				cell = currentCell;
			}
			if (in_quotes == false) { // Only add the cell if we have all the values (ie. not inside quotes)
				row.push(cell);
				cell = ""; // Reset the cell for the next run.
			}
		}
		csv.push(row);
		row = []; // Reset the row for the next run.
	}
	return csv;
}

