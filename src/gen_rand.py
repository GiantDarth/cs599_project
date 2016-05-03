import random
import json
from collections import OrderedDict


def random_sequence(size):
    seq = ''
    for i in range(size):
        seq += random.choice('ACGT')

    return seq


def generate_errors(subject, error_rate):
    subject_err = ''
    for c in range(len(subject)):
        if random.random() < error_rate:
            subject_err += random.choice('ACGT'.replace(subject[c], ''))
        else:
            subject_err += subject[c]

    return subject_err


def generate_read_indices(count, mer_len, subject):
    return random.sample(range(len(subject) - mer_len + 1), count)


if __name__ == '__main__':
    MER_LEN = 50
    READ_COUNT = 1000
    SUBJECT_SIZE = 1000000

    error_rates = [('low', 0.01), ('high', 0.05)]

    subj_tests = []
    for size in [10000, 100000, 1000000]:
        subject = random_sequence(size)
        subj_obj = OrderedDict([('subject', subject), ('size', size)])
        for error_rate in error_rates:
            subject_err = generate_errors(subject, error_rate[1])
            read_indices = generate_read_indices(READ_COUNT, MER_LEN, subject_err)
            reads = []
            for index in read_indices:
                reads.append(subject_err[index: index + MER_LEN])

            subj_obj[error_rate[0]] = OrderedDict([('rate', error_rate[1]), ('reads', reads)])
        subj_tests.append(subj_obj)

    f = open('subj_tests.json', 'w')
    f.write(json.dumps(subj_tests, sort_keys=False, indent=4))
    f.close()
    print("Created file " + f.name)

    subject = random_sequence(SUBJECT_SIZE)
    read_tests = OrderedDict([('subject', subject)])
    for error_rate in error_rates:
        read_tests[error_rate[0]] = OrderedDict([('rate', error_rate[1]), ('cases', [])])
        for count in [10, 100, 1000]:
            subject_err = generate_errors(subject, error_rate[1])

            read_indices = generate_read_indices(count, MER_LEN, subject_err)
            reads = []
            for index in read_indices:
                reads.append(subject_err[index: index + MER_LEN])

            read_tests[error_rate[0]]['cases'].append(reads)

    f = open('read_tests.json', 'w')
    f.write(json.dumps(read_tests, sort_keys=False, indent=4))
    f.close()
    print("Created file " + f.name)

    #generate subject with no errors in reads to test search methods
    subj_tests = []
    mersLen = 50
    for size in [10000, 100000, 1000000]:
        subject = random_sequence(size)
        subj_obj = OrderedDict([('subject', subject), ('size', size)])


        reads = []
        for index in range(int(size/1000)):
            tmpIndex = random.randrange(size-mersLen-1)
            reads.append(subject[tmpIndex:tmpIndex+mersLen])

        subj_obj['none'] = OrderedDict([('rate', 0), ('reads', reads)])
        subj_tests.append(subj_obj)

    f = open('subj_tests_no_errors.json', 'w')
    f.write(json.dumps(subj_tests, sort_keys=False, indent=4))
    f.close()
    print("Created file " + f.name)