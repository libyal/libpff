#!/usr/bin/env python3
"""Convert libpff Contact.txt exports to a merged vCard 3.0 .vcf file.

Usage:
    find ... -name Contact.txt | python3 debug/contact_to_vcf.py > contacts.vcf
    python3 debug/contact_to_vcf.py < file_list.txt > contacts.vcf
"""

import sys
import re

# Maps source phone label substrings to vCard TEL type tokens.
_PHONE_TYPE_MAP = {
    'mobile': 'CELL',
    'business': 'WORK',
    'home': 'HOME',
    'other': 'VOICE',
    'fax': 'FAX',
}


def _phone_vcf_type(label):
    """Return vCard TEL type token for a source phone field label."""
    label_lower = label.lower()
    for keyword, vcf_type in _PHONE_TYPE_MAP.items():
        if keyword in label_lower:
            return vcf_type
    return 'VOICE'


def is_real_email(value):
    """Return True if value looks like a real email address."""
    if not value:
        return False
    if value.startswith('/o=') or value.startswith('/O='):
        return False
    return bool(re.match(r'^[^@\s]+@[^@\s]+\.[^@\s]+$', value))


def _display_name(fields):
    """Derive a display name from available name fields."""
    if fields['display']:
        return fields['display']
    parts = []
    if fields['given']:
        parts.append(fields['given'])
    if fields['surname']:
        parts.append(fields['surname'])
    return ' '.join(parts)


def parse_contact(path):
    """Parse a single Contact.txt file into a field dict.

    Returns dict with keys: given, surname, display, file_under, initials,
    name_title, generational, company, job_title, office, department,
    locality, country, postal (list of lines), phones (list of (vcf_type, number)),
    emails (list of str).
    """
    try:
        with open(path, encoding='utf-8', errors='replace') as fh:
            lines = fh.readlines()
    except OSError:
        return None

    fields = {
        'given': '',
        'surname': '',
        'display': '',
        'file_under': '',
        'initials': '',
        'name_title': '',
        'generational': '',
        'company': '',
        'job_title': '',
        'office': '',
        'department': '',
        'locality': '',
        'country': '',
        'postal': [],
        'phones': [],   # list of (vcf_type, number)
        'emails': [],
    }

    in_contact = False
    in_postal = False
    postal_lines = []

    for line in lines:
        stripped = line.rstrip('\n')

        if stripped.strip() == 'Contact:':
            in_contact = True
            continue

        if stripped.strip() == 'Body:':
            if in_postal and postal_lines:
                fields['postal'] = postal_lines
            in_contact = False
            in_postal = False
            continue

        if not in_contact:
            continue

        # Postal address continuation: no label prefix
        if in_postal and not re.match(r'^[A-Za-z][\w\s]*:\s', stripped):
            if stripped.strip():
                postal_lines.append(stripped.strip())
            continue

        if in_postal and postal_lines:
            fields['postal'] = postal_lines
        in_postal = False

        m = re.match(r'^([A-Za-z][\w\s]*):\s+(.*)', stripped)
        if not m:
            continue

        label = m.group(1).strip()
        label_lower = label.lower()
        value = m.group(2).strip()

        if label_lower == 'given name':
            fields['given'] = fields['given'] or value
        elif label_lower == 'surname':
            fields['surname'] = fields['surname'] or value
        elif label_lower == 'display name':
            fields['display'] = fields['display'] or value
        elif label_lower == 'file under':
            fields['file_under'] = fields['file_under'] or value
        elif label_lower == 'initials':
            fields['initials'] = fields['initials'] or value
        elif label_lower == 'title':
            fields['name_title'] = fields['name_title'] or value
        elif label_lower == 'generational abbreviation':
            fields['generational'] = fields['generational'] or value
        elif label_lower == 'company name':
            fields['company'] = fields['company'] or value
        elif label_lower == 'job title':
            fields['job_title'] = fields['job_title'] or value
        elif label_lower == 'office location':
            fields['office'] = fields['office'] or value
        elif label_lower == 'department name':
            fields['department'] = fields['department'] or value
        elif label_lower == 'locality':
            fields['locality'] = fields['locality'] or value
        elif label_lower == 'country':
            fields['country'] = fields['country'] or value
        elif label_lower == 'postal address':
            postal_lines = [value] if value else []
            in_postal = True
        elif re.search(r'phone|fax', label_lower):
            if value:
                vcf_type = _phone_vcf_type(label)
                if (vcf_type, value) not in fields['phones']:
                    fields['phones'].append((vcf_type, value))
        elif re.match(r'email address \d', label_lower):
            if is_real_email(value) and value.lower() not in [e.lower() for e in fields['emails']]:
                fields['emails'].append(value)

    if in_postal and postal_lines:
        fields['postal'] = postal_lines

    return fields


def contact_key(fields):
    """Return the merge key for a contact record.

    Keyed on first real email (lowercased) when present, else display name.
    """
    if fields['emails']:
        return ('email', fields['emails'][0].lower())
    display = _display_name(fields)
    return ('display', display.lower()) if display else None


def merge_into(base, incoming):
    """Merge incoming fields into base, accumulating phones and emails."""
    scalar_fields = (
        'given', 'surname', 'display', 'file_under', 'initials',
        'name_title', 'generational', 'company', 'job_title',
        'office', 'department', 'locality', 'country',
    )
    for f in scalar_fields:
        if not base[f] and incoming[f]:
            base[f] = incoming[f]

    if not base['postal'] and incoming['postal']:
        base['postal'] = incoming['postal']

    for phone_entry in incoming['phones']:
        if phone_entry not in base['phones']:
            base['phones'].append(phone_entry)

    for email in incoming['emails']:
        if email.lower() not in [e.lower() for e in base['emails']]:
            base['emails'].append(email)


def vcard_escape(value):
    """Escape special characters per vCard 3.0 value encoding rules."""
    value = value.replace('\\', '\\\\')
    value = value.replace(',', '\\,')
    value = value.replace(';', '\\;')
    value = value.replace('\n', '\\n')
    return value


def format_vcard(fields):
    """Render a merged contact dict as a vCard 3.0 string."""
    output_lines = ['BEGIN:VCARD', 'VERSION:3.0']

    given = vcard_escape(fields['given'])
    surname = vcard_escape(fields['surname'])
    prefix = vcard_escape(fields['name_title'])
    suffix = vcard_escape(fields['generational'])
    initials = vcard_escape(fields['initials'])

    display = _display_name(fields) or 'Unknown'
    output_lines.append('FN:' + vcard_escape(display))

    # N: surname;given;additional;prefix;suffix
    output_lines.append(f'N:{surname};{given};{initials};{prefix};{suffix}')

    if fields['company']:
        org_parts = [vcard_escape(fields['company'])]
        if fields['department']:
            org_parts.append(vcard_escape(fields['department']))
        output_lines.append('ORG:' + ';'.join(org_parts))

    if fields['job_title']:
        output_lines.append('TITLE:' + vcard_escape(fields['job_title']))

    for vcf_type, number in fields['phones']:
        output_lines.append(f'TEL;TYPE={vcf_type}:{vcard_escape(number)}')

    for email in fields['emails']:
        output_lines.append('EMAIL;TYPE=INTERNET:' + vcard_escape(email))

    if fields['postal'] or fields['locality'] or fields['country']:
        street = '\\n'.join(vcard_escape(ln) for ln in fields['postal'])
        locality = vcard_escape(fields['locality'])
        country = vcard_escape(fields['country'])
        # ADR: pobox;ext;street;locality;region;postalcode;country
        output_lines.append(f'ADR;TYPE=WORK:;;{street};{locality};;;{country}')

    output_lines.append('END:VCARD')
    return '\r\n'.join(output_lines) + '\r\n'


def _log(msg):
    """Write an informational line to stderr."""
    print(msg, file=sys.stderr)


def main():
    contacts = {}  # key -> fields dict
    n_inputs = 0
    n_parse_errors = 0
    n_no_key = 0
    n_merged = 0

    for raw_path in sys.stdin:
        path = raw_path.strip()
        if not path:
            continue

        n_inputs += 1
        fields = parse_contact(path)
        if fields is None:
            n_parse_errors += 1
            _log(f'error: could not read {path}')
            continue

        key = contact_key(fields)
        if key is None:
            n_no_key += 1
            _log(f'skip: no key (no email, no name) in {path}')
            continue

        if key in contacts:
            n_merged += 1
            merge_into(contacts[key], fields)
        else:
            contacts[key] = fields

    for fields in contacts.values():
        sys.stdout.write(format_vcard(fields))

    n_outputs = len(contacts)
    _log('')
    _log(f'inputs read:       {n_inputs}')
    _log(f'parse errors:      {n_parse_errors}')
    _log(f'skipped (no key):  {n_no_key}')
    _log(f'merged duplicates: {n_merged}')
    _log(f'unique contacts:   {n_outputs}')
    _log(f'accounted:         {n_parse_errors + n_no_key + n_merged + n_outputs}'
         f' (expect {n_inputs})')


if __name__ == '__main__':
    main()
