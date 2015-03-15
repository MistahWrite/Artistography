var $ = jQuery.noConflict();

$(document).ready(function() {

	if ( $( "#ordersTable" ).length != 0 ) {

		function viewOrder(view_order_id) {
			var data_edit = {
				action: 'View_Order',
				order_id: view_order_id
			};

			$.post(ajaxurl, data_edit, function(response) {
				var res = response.split("##$$##");
				$( "#dialog-form" ).dialog( "open" );
				$( "#id" ).val(res[0]);
				$( "#name" ).val(res[1]);
				$( "#email" ).val(res[2]);
				$( "#street_address" ).val(res[3]);
				$( "#town" ).val(res[4]);
				$( "#state" ).val(res[5]);
				$( "#country" ).val(res[6]);
				$( "#contact_phone" ).val(res[7]);
				$( "#payer_id" ).val(res[8]);
				$( "#address_status" ).val(res[9]);
				$( "#payer_status" ).val(res[10]);
				$( "#items_number_ordered" ).val(res[11]);
				$( "#quantity_ordered" ).val(res[12]);
				$( "#txn_type" ).val(res[13]);
				$( "#parent_txn_id" ).val(res[14]);
				$( "#verify_sign" ).val(res[15]);
				$( "#quantity" ).val(res[16]);
				$( "#shipping" ).val(res[17]);
				$( "#mc_fee" ).val(res[18]);
				$( "#mc_gross" ).val(res[19]);
				$( "#mc_shipping" ).val(res[20]);
				$( "#mc_handling" ).val(res[21]);
				$( "#tax" ).val(res[22]);
				$( "#refund_reason" ).val(res[23]);
				$( "#refund_receipt_id" ).val(res[24]);
				$( "#memo" ).val(res[25]);
				$( "#pending_reason" ).val(res[26]);
				$( "#postcode" ).val(res[27]);
				$( "#items_ordered" ).val(res[28]);
				$( "#created" ).val(res[29]);
				$( "#txn_id" ).val(res[30]);
				$( "#custom" ).val(res[31]);
				$( "#payment_status" ).val(res[32]);
				$( "#payment_amount" ).val(res[33]);
				$( "#payment_currency" ).val(res[34]);
				$( "#item_name" ).val(res[35]);
				$( "#item_number" ).val(res[36]);
				$( "#user_id" ).val(res[37]);
			});
		}

		$( ".view_button" ).button({
			text: true,
			icons: {
				primary: "ui-icon-zoomin"
			}
		}).click(function() {
			var id = $( this ).attr('id').replace('view_', '');
			viewOrder(id);
		});

		zebraRows('tbody tr:odd td', 'odd');

		$('tbody tr').hover(function(){
			$(this).find('td').addClass('hovered');
		}, function(){
			$(this).find('td').removeClass('hovered');
		});

		//default each row to visible
		$('tbody tr').addClass('visible');

		$('#filter').show();

		$('#filter').keyup(function(event) {
			 //if esc is pressed or nothing is entered
			if (event.keyCode == 27 || $(this).val() == '') {
				 //if esc is pressed we want to clear the value of search box
				$(this).val('');

				 //we want each row to be visible because if nothing   
				 //is entered then all rows are matched.   
				$('tbody tr').removeClass('visible').show().addClass('visible'); 
			} else { //if there is text, lets filter
				filter('tbody tr', $(this).val());
			}

			 //reapply zebra rows
			$('.visible td').removeClass('odd');
			zebraRows('.visible:odd td', 'odd');
		});

		 // ***** START SORTING CODE ********/
		 //grab all header rows   
		$('thead th').each(function(column) {   
			$(this).addClass('sortable').click(function(){   
				var findSortKey = function($cell) {   
					return $cell.find('.sort-key').text().toUpperCase() + ' ' + $cell.text().toUpperCase();   
				};   
				var sortDirection = $(this).is('.sorted-asc') ? -1 : 1;   
      
				 //step back up the tree and get the rows with data   
				 //for sorting   
				var $rows = $(this).parent().parent().parent().find('tbody tr').get();   
      
				 //loop through all the rows and find   
				$.each($rows, function(index, row) {   
					row.sortKey = findSortKey($(row).children('td').eq(column));   
				});   
      
				 //compare and sort the rows alphabetically   
				$rows.sort(function(a, b) {   
					if (a.sortKey < b.sortKey) return -sortDirection;   
					if (a.sortKey > b.sortKey) return sortDirection;   
					return 0;   
				});
      
				 //add the rows in the correct order to the bottom of the table   
				$.each($rows, function(index, row) {   
					$('tbody').append(row);   
					row.sortKey = null;   
				});   

				 //identify the column sort order   
				$('th').removeClass('sorted-asc sorted-desc');   
				var $sortHead = $('th').filter(':nth-child(' + (column + 1) + ')');   
				sortDirection == 1 ? $sortHead.addClass('sorted-asc') : $sortHead.addClass('sorted-desc');   
      
				 //identify the column to be sorted by   
				$('td').removeClass('sorted')   
					.filter(':nth-child(' + (column + 1) + ')')   
					.addClass('sorted');   
        
				$('.visible td').removeClass('odd');   
				zebraRows('.visible:even td', 'odd');   
			});
		});
	} /* end if ( $( "#ordersTable" ).length != 0 ) */

	if ( $( "#dialog-form" ).length != 0) {
		 // a workaround for a flaw in the demo system (http://dev.jqueryui.com/ticket/4375), ignore!
		$( "#dialog:ui-dialog" ).dialog( "destroy" );

		var id = $( "#id" ),
			name = $( "#name" ),
			emaill = $( "#email" ),
			street_address = $( "#street_address" ),
			town = $( "#town" ),
			state = $( "#state" ),
                        country = $( "#country" ),
                        contact_phone = $( "#contact_phone" ),
                        payer_id = $( "#payer_id" ),
                        address_status = $( "#address_status" ),
                        payer_status = $( "#payer_status" ),
                        items_number_ordered = $( "#items_number_ordered" ),
                        quantity_ordered = $( "#quantity_ordered" ),
                        txn_type = $( "#txn_type" ),
                        parent_txn_id =$( "#parent_txn_id" ),
                        verify_sign =$( "#verify_sign" ),
                        quantity = $( "#quantity" ),
                        shipping = $( "#shipping" ),
                        mc_fee = $( "#mc_fee" ),
                        mc_gross = $( "#mc_gross" ),
                        mc_shipping = $( "#mc_shipping" ),
                        tax = $( "#tax" ),
                        refund_reason_code = $( "#refund_reason" ),
                        refund_receipt_id = $( "#refund_receipt_id" ),
                        memo = $( "#memo" ),
                        pending_reason = $( "#pending_reason" ),
                        postcode = $( "#postcode" ),
                        items_ordered = $( "#items_ordered" ),
                        created = $( "#created" ),
                        txn_id = $( "#txn_id" ),
                        custom = $( "#custom" ),
                        payment_status = $( "#payment_status" ),
                        payment_amount = $( "#payment_amount" ),
                        payment_currency = $( "#payment_currency" ),
                        item_name = $( "#item_name" ),
                        item_number = $( "#item_number" ),
                        user_id = $( "#user_id" ),
			allFields = $( [] )
				.add( id )
				.add( name )
				.add( email )
				.add( street_address )
				.add( town )
				.add( state )
                                .add( country )
                                .add( contact_phone )
                                .add( payer_id )
                                .add( address_status )
                                .add( payer_status )
                                .add( items_number_ordered )
                                .add( quantity_ordered )
                                .add( txn_type )
                                .add( parent_txn_id )
                                .add( verify_sign )
                                .add( quantity )
                                .add( shipping )
                                .add( mc_fee )
                                .add( mc_gross )
                                .add( mc_shipping )
                                .add( tax )
                                .add( refund_reason_code )
                                .add( refund_receipt_id )
                                .add( memo )
                                .add( pending_reason )
                                .add( postcode )
                                .add( items_ordered )
                                .add( created )
                                .add( txn_id )
                                .add( custom )
                                .add( payment_status )
                                .add( payment_amount )
                                .add( payment_currency )
                                .add( item_name )
                                .add( item_number )
                                .add( user_id );

		$( "#dialog-form" ).dialog({
			autoOpen: false,
			show: "implode",
			hide: "explode",
			height: 500,
			width: 350,
			modal: true,
			buttons: {
				"Close": function() {
					$( this ).dialog( "close" );
				}
			},
			close: function(ev, ui) {
				$(this).hide();
				allFields.val( "" ).removeClass( "ui-state-error" );
				zebraRows('.visible:even td', 'odd')
			}
		});
	}
});

 //used to apply alternating row styles
function zebraRows(selector, className)
{
	$(selector).removeClass(className).addClass(className);
}

 //filter results based on query   
function filter(selector, query) {
	query = $.trim(query); //trim white space
	query = query.replace(/ /gi, '|'); //add OR for regex query

	$(selector).each(function() {
		($(this).text().search(new RegExp(query, "i")) < 0) ? $(this).hide().removeClass('visible') : $(this).show().addClass('visible');
	});
}
