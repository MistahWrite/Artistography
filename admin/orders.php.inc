<?php

GLOBAL $wpdb, $i18n_domain, $TABLE_NAME;

$orders = new Orders;
$i = 0;
echo "<div class='aform'><div id='dialog-form' title='" . __("Order", $i18n_domain)."' style='display:none;overflow:auto'>
     <form>
     <fieldset>
          <input type='hidden' name='id' id='id' />
	<label for='name'>" .__("Name", $i18n_domain). "</label>
	  <input readonly type='text' name='name' id='name' class='text ui-widget-content ui-corner-all' />
	<label for='street_address'>" .__("Street Address", $i18n_domain). "</label>
          <input readonly type='text' name='street_address' id='street_address' class='text ui-widget-content ui-corner-all' />
	<label for='postcode'>" .__("Postcode", $i18n_domain). "</label>
          <input readonly type='text' name='postcode' id='postcode' class='text ui-widget-content ui-corner-all' />
	<label for='town'>" .__("Town", $i18n_domain). "</label>
          <input readonly type='text' name='town' id='town' class='text ui-widget-content ui-corner-all' />
	<label for='state'>" .__("State", $i18n_domain). "</label>
          <input readonly type='text' name='state' id='state' class='text ui-widget-content ui-corner-all' />
	<label for='country'>" .__("Country", $i18n_domain). "</label>
          <input readonly type='text' name='country' id='country' class='text ui-widget-content ui-corner-all' />
	<label for='contact_phone'>" .__("Contact Phone", $i18n_domain). "</label>
          <input readonly type='text' name='contact_phone' id='contact_phone' class='text ui-widget-content ui-corner-all' />
  
        <label for='email'>" .__("Email", $i18n_domain). "</label>
          <input readonly type='text' name='email' id='email' class='text ui-widget-content ui-corner-all' />
        <label for='payer_id'>" .__("Payer ID", $i18n_domain). "</label>
          <input readonly type='text' name='payer_id' id='payer_id' class='text ui-widget-content ui-corner-all' />
        <label for='address_status'>" .__("Address Status", $i18n_domain). "</label>
          <input readonly type='text' name='address_status' id='address_status' class='text ui-widget-content ui-corner-all' />
        <label for='Payer Status'>" .__("Payer Status", $i18n_domain). "</label>
          <input readonly type='text' name='payer_status' id='payer_status' class='text ui-widget-content ui-corner-all' /

	<label for='items_ordered'>" .__("Items Ordered", $i18n_domain). "</label>
		<textarea name='products_purchased' id='items_ordered' style='width:100%' rows='10' class='text ui-widget-content ui-corner-all'/></textarea>
	<label for='items_number_ordered'>" .__("Items Number Ordered", $i18n_domain). "</label>
          <input readonly type='text' name='items_number_ordered' id='items_number_ordered' class='text ui-widget-content ui-corner-all' />
	<label for='quantity_ordered'>" .__("Quantity Ordered", $i18n_domain). "</label>
          <input readonly type='text' name='quantity_ordered' id='quantity_ordered' class='text ui-widget-content ui-corner-all' />
	<label for='created'>" .__("Created", $i18n_domain). "</label>
          <input readonly type='text' name='created' id='created' class='text ui-widget-content ui-corner-all' />
	<label for='txn_id'>" .__("Transmission ID", $i18n_domain). "</label>
          <input readonly type='text' name='txn_id' id='txn_id' class='text ui-widget-content ui-corner-all' />
	<label for='txn_type'>" .__("Transmission Type", $i18n_domain). "</label>
          <input readonly type='text' name='txn_type' id='txn_type' class='text ui-widget-content ui-corner-all' />
	<label for='parent_txn_id'>" .__("Parent Transmission ID", $i18n_domain). "</label>
          <input readonly type='text' name='parent_txn_id' id='parent_txn_id' class='text ui-widget-content ui-corner-all' />
	<label for='verify_sign'>" .__("Verify Sign", $i18n_domain). "</label>
          <input readonly type='text' name='verify_sign' id='verify_sign' class='text ui-widget-content ui-corner-all' />
	<label for='payment_status'>" .__("Payment Status", $i18n_domain). "</label>
          <input readonly type='text' name='payment_status' id='payment_status' class='text ui-widget-content ui-corner-all' />
	<label for='payment_amount'>" .__("Payment Amount", $i18n_domain). "</label>
          <input readonly type='text' name='payment_amount' id='payment_amount' class='text ui-widget-content ui-corner-all' />
	<label for='payment_currency'>" .__("Payment Currency", $i18n_domain). "</label>
          <input readonly type='text' name='payment_currency' id='payment_currency' class='text ui-widget-content ui-corner-all' />
	<label for='item_name'>" .__("Item Name", $i18n_domain). "</label>
          <input readonly type='text' name='item_name' id='item_name' class='text ui-widget-content ui-corner-all' />
	<label for='item_number'>" .__("Item Number", $i18n_domain). "</label>
          <input readonly type='text' name='item_number' id='item_number' class='text ui-widget-content ui-corner-all' />
	<label for='quantity'>" .__("Quantity", $i18n_domain). "</label>
          <input readonly type='text' name='quantity' id='quantity' class='text ui-widget-content ui-corner-all' />
	<label for='shipping'>" .__("Shipping", $i18n_domain). "</label>
          <input readonly type='text' name='shipping' id='shipping' class='text ui-widget-content ui-corner-all' />
	<label for='tax'>" .__("Tax", $i18n_domain). "</label>
          <input readonly type='text' name='tax' id='tax' class='text ui-widget-content ui-corner-all' />
	<label for='mc_fee'>" .__("MC Fee", $i18n_domain). "</label>
          <input readonly type='text' name='mc_fee' id='mc_fee' class='text ui-widget-content ui-corner-all' />
	<label for='mc_gross'>" .__("MC Gross", $i18n_domain). "</label>
          <input readonly type='text' name='mc_gross' id='mc_gross' class='text ui-widget-content ui-corner-all' />
	<label for='mc_handling'>" .__("MC Handling", $i18n_domain). "</label>
          <input readonly type='text' name='mc_handling' id='mc_handling' class='text ui-widget-content ui-corner-all' />
	<label for='mc_shipping'>" .__("MC Shipping", $i18n_domain). "</label>
          <input readonly type='text' name='mc_shipping' id='mc_shipping' class='text ui-widget-content ui-corner-all' />
	<label for='Memo'>" .__("Memo", $i18n_domain). "</label>
          <input readonly type='text' name='memo' id='memo' class='text ui-widget-content ui-corner-all' />
	<label for='pending_reason'>" .__("Pending Reason", $i18n_domain). "</label>
          <input readonly type='text' name='pending_reason' id='pending_reason' class='text ui-widget-content ui-corner-all' />
	<label for='Refund Reason'>" .__("Refund Reason", $i18n_domain). "</label>
          <input readonly type='text' name='refund_reason' id='refund_reason' class='text ui-widget-content ui-corner-all' />
	<label for='Refund Receipt ID'>" .__("Refund Receipt ID", $i18n_domain). "</label>
          <input readonly type='text' name='refund_receipt_id' id='refund_receipt_id' class='text ui-widget-content ui-corner-all' />
	<label for='user_id'>" .__("User ID", $i18n_domain). "</label>
	  <input readonly type='text' name='user_id' id='user_id' class='text ui-widget-content ui-corner-all' />
  </fieldset>
  </form>
  </div>";
echo "  <div style='float:left;'><h1>" .__('Processed Orders', $i18n_domain). ":</h1></div>
        <div style='float:right;border:1px dotted black; padding:10px;'>
	    <label for='filter'>" .__('Filter:', $i18n_domain). "</label>
	    <input type='text' name='filter' value='' id='filter' /></div>
<table id='ordersTable' class='ui-widget ui-widget-content'>
	<thead>
		<tr>
			<th>#</td>
			<th>" .__("Forename", $i18n_domain). "</th>
			<th>" .__("Surname", $i18n_domain). "</th>
			<th>" .__("Email", $i18n_domain). "</th>
			<th>" .__("Products purchased", $i18n_domain). "</th>
			<th>" .__("Status", $i18n_domain). "</th>
			<th>" .__("Transmission ID", $i18n_domain). "</th>
			<th>" .__("Amount", $i18n_domain). "</th>
			<th>" .__("User ID", $i18n_domain). "</th>
			<th>" .__("Actions", $i18n_domain). "</th>
		</tr>
	</thead>
	<tbody>";
	if ($orders->getTotalRows() > 0) {
		do {
		$products = unserialize($orders->itemsOrdered);
		echo "	<tr>
				<td>$orders->id</td>
				<td>$orders->forename</td>
				<td>$orders->surname</td>
				<td>$orders->email</td>
				<td>";
		echo "			<ol>";
		for($j = 0; $j < count($products); $j++) {
			echo "			<li>" .$products[$j]. "</li>";
		}
		echo "			</ol>";
		echo	"	</td>
				<td>$orders->payment_status</td>
				<td>$orders->txn_id</td>
				<td>$orders->payment_amount</td>
				<td>$orders->user_id</td>
				<td>
					<div style='float:left;'>
						<button class='view_button' id='view_$orders->id'>" .__('View', $i18n_domain). "</button>
					</div>
				</td>
			</tr>";
		$orders->getNodeNext();
		$i++;
		} while($i<$orders->getTotalRows());
	} else {
		echo "<tr><td colspan='10'>There are no orders yet.</td></tr>";
	}
echo "	</tbody>
</table>";

?>
